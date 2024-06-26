#include <ConfigParser.h>


ConfigParser::ConfigParser(std::string config_file){
    config = YAML::LoadFile(config_file);
    AudioMatrixConfig audio_matrix_config;

    // read global config options
    if (config["port"].IsDefined() && config["port"].IsScalar()){
        audio_matrix_config.port = config["port"].as<int>();
    }
    
    if (config["n_input_channels"].IsDefined() && config["n_input_channels"].IsScalar()){
        audio_matrix_config.n_input_channels = config["n_input_channels"].as<int>();
    } 
    
    // check if the tracks section exists in the config, and is a sequence
    YAML::Node tracksNode = config["tracks"];
    if (tracksNode.IsDefined() && tracksNode.IsSequence()){
                
        // iterate over all tracks
        for (YAML::const_iterator t_it=tracksNode.begin(); t_it!=tracksNode.end(); ++t_it){
            YAML::Node track = t_it->as<YAML::Node>(); // convert iterator back to node
            TrackConfig track_config;

            if (track["name"].IsDefined()){
                track_config.name = track["name"].as<std::string>();
                std::cout << "[info] Parsing track " << track_config.name << std::endl;
            } else {
                std::cout << "[warning] Track has no name" << std::endl;
            }

            
            YAML::Node track_modules = track["modules"];
            std::vector<ModuleConfig> modules;

            // iterate over all modules of this track
            if (track_modules.IsDefined() && track_modules.IsSequence()){
                for (YAML::const_iterator module_it=track_modules.begin(); module_it!=track_modules.end(); ++module_it){
                    YAML::Node module = module_it->as<YAML::Node>();
                    
                    // parse module config, push it to the track_config if it is valid
                    std::shared_ptr<ModuleConfig> module_config = parse_module(module);
                    if (module_config != nullptr){
                        track_config.modules.push_back(module_config);
                    }
                }
            } else {
                std::cout << "[warning] Track has no Modules" << std::endl;
            }
            audio_matrix_config.tracks.push_back(track_config);
        }
    } else {
        std::cout << "[warning] No tracks defined" << std::endl;
    }
    parsed_config = std::make_shared<AudioMatrixConfig>(audio_matrix_config);
}

std::shared_ptr<ModuleConfig> ConfigParser::parse_module(YAML::Node module){
    std::string name;

    if (module.IsMap()){
        name = module.begin()->first.as<std::string>(); 
    } else if (module.IsScalar()){
        name = module.as<std::string>(); 
    }

    if (name == "gain"){
        return parse_module_gain(module);
    } else if (name == "filter") {
        return parse_module_filter(module);
    } else if (name == "hoa_encoder") {
        return parse_module_ambi_encoder(module);
    } else {
        std::cout << "[error] Unknown module: " << name << std::endl;
        return nullptr;
    }
    
}

void ConfigParser::parse_module_osc_params(YAML::Node module, std::shared_ptr<ModuleConfig> config){
    if (!module.IsMap()){
        return;
    }

    YAML::Node moduleConfigNode = module.begin()->second;
    if (!moduleConfigNode.IsMap()){
        // return if there are no secondary parameters
        return;
    }
    
    // get value of "osc_controllable"
    YAML::Node oscControllableNode = moduleConfigNode["osc_controllable"];
    if (oscControllableNode.IsDefined()){
        config->osc_controllable = oscControllableNode.as<bool>();
    }
    // get value of "osc_paths"
    YAML::Node oscPathNode = moduleConfigNode["osc_paths"];
    if (config->osc_controllable && oscPathNode.IsDefined()){
        // TODO handle paths being a list
        config->osc_path = oscPathNode.as<std::string>();
    } else if (!config->osc_controllable && oscPathNode.IsDefined()){
        std::cout << "[error] OSC path missing on module with activated osc receiver" << std::endl;
    }
}

ModuleConfigPtr ConfigParser::parse_module_gain(YAML::Node module){
    // Create config file
    GainConfigPtr config = std::make_shared<GainConfig>();

    // parse basic osc params
    parse_module_osc_params(module, config);
    if(config->osc_controllable){
        std::cout << "[info] Module Gain listens to OSC on path: " << config->osc_path << std::endl;
    }

    // get config values from config
    if (module.IsMap()){
        YAML::Node moduleConfigNode = module.begin()->second;
        if (moduleConfigNode.IsMap()){
            if (moduleConfigNode["factor"].IsDefined()){
                config->factor = moduleConfigNode["factor"].as<float>();
            }
        } else {
            config->factor = moduleConfigNode.as<float>();
        }
    }
    return config;
}

ModuleConfigPtr ConfigParser::parse_module_filter(YAML::Node module){
    FilterConfigPtr config = std::make_shared<FilterConfig>();
    parse_module_osc_params(module, config);
    if(!module.IsMap()){
        std::cout << "Filter missing parameters" << std::endl;
        return nullptr;
    }
    
    YAML::Node moduleConfigNode = module.begin()->second;
    if (!moduleConfigNode.IsMap()){
        std::cout << "Filter missing parameters" << std::endl;
        return nullptr;
    }

    // Get Filter Type from Config
    if (moduleConfigNode["type"].IsDefined()){
        std::string filter_type = moduleConfigNode["type"].as<std::string>();
        if (filter_type == "HP"){
            config->type = FilterType::HP;
        } else if (filter_type == "LP"){
            config->type = FilterType::LP;
        } else if (filter_type == "BP"){
            config->type = FilterType::BP;
        } else {
            std::cout << "Unknown Filter Type: " << filter_type << std::endl;
        }
    } else {
        std::cout << "Filter Type unspecified" << std::endl;
    }

    if (moduleConfigNode["freq"].IsDefined()){
        config->freq = moduleConfigNode["freq"].as<float>();
    } else {
        std::cout << "Filter Frequency undefined" << std::endl;
    }

    return config;
}

ModuleConfigPtr ConfigParser::parse_module_ambi_encoder(YAML::Node module){
    AmbiEncoderConfigPtr config = std::make_shared<AmbiEncoderConfig>();
    parse_module_osc_params(module, config);
    if(!module.IsMap()){
        std::cout << "Ambisonics encoder missing parameters" << std::endl;
        return nullptr;
    }

    YAML::Node moduleConfigNode = module.begin()->second;
    if (!moduleConfigNode.IsMap()){
        std::cout << "Ambisonics encoder missing parameters" << std::endl;
        return nullptr;
    }

    if (moduleConfigNode["order"].IsDefined()){
        config->order = moduleConfigNode["order"].as<int>();
    } else {
        std::cout << "Ambisonics encoder order unspecified" << std::endl;
    }
    return config;
}

ConfigParser::~ConfigParser(){
}

std::shared_ptr<AudioMatrixConfig> ConfigParser::get_config(){
    return parsed_config;
}
