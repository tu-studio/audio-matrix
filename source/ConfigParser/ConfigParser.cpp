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

ConfigParser::~ConfigParser(){
}

std::shared_ptr<AudioMatrixConfig> ConfigParser::get_config(){
    return parsed_config;
}


template <typename T>
T ConfigParser::get_config_option(YAML::Node module, const std::string &option_name, T default_value, bool is_main_parameter, bool is_required, bool warn_when_default){
    
    if (module.IsMap()){
        YAML::Node moduleConfigNode = module.begin()->second;
        if (moduleConfigNode.IsMap()){
            if (moduleConfigNode[option_name].IsDefined()){
                try {
                    return moduleConfigNode[option_name].as<T>();
                } catch (const YAML::BadConversion& e){
                    std::cout << "[warn] Invalid value for parameter " << option_name << std::endl;
                }
            }
        } else if (is_main_parameter) {
            // special case for modules that have just one config option, which can be specified in the config file using "module_name: value"
            try {
                return moduleConfigNode.as<T>();
            } catch (const YAML::BadConversion& e){
                std::cout << "[warn] Invalid value for parameter " << option_name << std::endl;
            }
        }
    }
    if (is_required) {
        std::cout << "[error] Module missing required parameter " << option_name << std::endl;
        // TODO exit here
    }
    if (warn_when_default)
        std::cout << "[warn] using default value \"" << default_value << "\"" << std::endl;
    return default_value;
}

std::shared_ptr<ModuleConfig> ConfigParser::parse_module(YAML::Node module){
    std::string name;

    // get name of module
    if (module.IsMap()){
        name = module.begin()->first.as<std::string>(); 
    } else if (module.IsScalar()){
        name = module.as<std::string>(); 
    }

    std::cout << "[debug] \t parsing module: " << name << std::endl;
    // MODULE PARSER CASES GO HERE
    if (name == "gain"){
        return parse_module_gain(module);
    } else if (name == "filter") {
        return parse_module_filter(module);
    } else if (name == "hoa_encoder") {
        return parse_module_ambi_encoder(module);
    } else if (name == "sum") {
        return parse_module_sum(module);
    } 
    // END MODULE PARSER CASES
    else {
        std::cout << "[error] Unknown module: " << name << std::endl;
        return nullptr;
    }
    
}

void ConfigParser::parse_module_osc_params(YAML::Node module, std::shared_ptr<ModuleConfig> config){
    
    std::string osc_path = get_config_option<std::string>(module, "osc_path", "", false, false, false);
    if (osc_path != ""){
        config->osc_controllable = true;
        config->osc_path = osc_path;
    }
}

// CUSTOM MODULE PARSERS GO HERE

ModuleConfigPtr ConfigParser::parse_module_gain(YAML::Node module){
    // create config
    GainConfigPtr config = std::make_shared<GainConfig>();

    // parse basic osc params
    parse_module_osc_params(module, config);

    // get config values from config
    config->factor = get_config_option<float>(module, "factor", 1, true);

    return config;
}

ModuleConfigPtr ConfigParser::parse_module_sum(YAML::Node module){
    SumConfigPtr config = std::make_shared<SumConfig>();
    return config;
}


ModuleConfigPtr ConfigParser::parse_module_filter(YAML::Node module){
    FilterConfigPtr config = std::make_shared<FilterConfig>();
    parse_module_osc_params(module, config);
    
    // Get Filter Type from Config
    std::string filter_type = get_config_option<std::string>(module, "type", "HP", true);

    if (filter_type == "HP"){
        config->type = FilterType::HP;
    } else if (filter_type == "LP"){
        config->type = FilterType::LP;
    } else {
        std::cout << "Unknown Filter Type: " << filter_type << std::endl;
    }

    config->freq = get_config_option<float>(module, "freq", 150);
    config->order = get_config_option<int>(module, "order", 4);

    return config;
}

ModuleConfigPtr ConfigParser::parse_module_ambi_encoder(YAML::Node module){
    AmbiEncoderConfigPtr config = std::make_shared<AmbiEncoderConfig>();
    
    parse_module_osc_params(module, config);

    config->order = get_config_option<int>(module, "order", 3, true);
    
    return config;
}

