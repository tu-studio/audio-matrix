#include <ConfigParser.h>


ConfigParser::ConfigParser(std::string config_file){
    config = YAML::LoadFile(config_file);


    if (config["port"].IsDefined() && config["port"].IsScalar()){
        port = config["port"].as<int>();
        std::cout << "Port: " << port << std::endl;
    } else {
        port = DEFAULT_PORT;
        std::cout << "port not configured, using default: " << port << std::endl;
    }
    
    if (config["n_input_channels"].IsDefined() && config["n_input_channels"].IsScalar()){
        n_input_channels = config["n_input_channels"].as<int>();
        std::cout << "n_input_channels: " << n_input_channels << std::endl;
    } else {
        n_input_channels = DEFAULT_INPUT_PORTS;
        std::cout << "n_input_channels not configured, using default: " << n_input_channels << std::endl;
    }

    std::string osc_base_path;
    if (config["osc_base_path"].IsDefined() && config["osc_base_path"].IsScalar()){
        osc_base_path = config["osc_base_path"].as<std::string>();
        std::cout << "osc_base_path: " << osc_base_path << std::endl;
    } else {
        osc_base_path = DEFAULT_OSC_BASE_PATH;
        std::cout << "osc_base_path not configured, using default: " << osc_base_path << std::endl;
    }

    std::string osc_pos_path;
    if (config["osc_position_path"].IsDefined() && config["osc_position_path"].IsScalar()){
        osc_pos_path = config["osc_position_path"].as<std::string>();
        std::cout << "osc_position_path: " << osc_pos_path << std::endl;
    } else {
        osc_pos_path = DEFAULT_OSC_POS_PATH;
        std::cout << "osc_position_path not configured, using default: " << osc_pos_path << std::endl;
    }

    
    
    
    // check if the tracks section exists in the config, and is a sequence
    std::vector<TrackConfig> tracks;
    YAML::Node tracksNode = config["tracks"];
    if (tracksNode.IsDefined() && tracksNode.IsSequence()){
        
        std::cout << "Number of tracks: " << tracksNode.size() << std::endl;
        
        // iterate over all tracks
        for (YAML::const_iterator t_it=tracksNode.begin(); t_it!=tracksNode.end(); ++t_it){
            YAML::Node track = t_it->as<YAML::Node>();

            std::string track_name;
            if (track["name"].IsDefined()){
                track_name = track["name"].as<std::string>();
                std::cout << track_name << std::endl;
            }

            YAML::Node track_modules = track["modules"];
            
            std::vector<ModuleConfig> modules;
            if (track_modules.IsDefined() && track_modules.IsSequence()){
                for (YAML::const_iterator module_it=track_modules.begin(); module_it!=track_modules.end(); ++module_it){
                    YAML::Node module = module_it->as<YAML::Node>();
                    if (module.IsMap()){
                        std::cout << "\t" << module.begin()->first.as<std::string>() << std::endl; 
                        YAML::Node moduleConfigNode = module.begin()->second;
                        if (moduleConfigNode.IsMap()){
                            for (YAML::const_iterator module_cfg_it = moduleConfigNode.begin(); module_cfg_it!=moduleConfigNode.end(); ++module_cfg_it){
                                std::cout << "\t\t" << module_cfg_it->first << ": " << module_cfg_it->second << std::endl;
                            }
                        }
                    } else if (module.IsScalar()){
                        std::cout << "\t" << module.as<std::string>() << std::endl; 
                    }
                }
            } else {
                std::cout << "[WARNING] Track has no Modules" << std::endl;
            }
            TrackConfig track_conf(track_name, modules);
            tracks.push_back(track_conf);
        }
    } else {
        std::cout << "[WARNING] No tracks defined" << std::endl;
    }
    AudioMatrixConfig config(port, n_input_channels, osc_base_path, osc_pos_path, tracks);
    parsed_config = std::make_shared<AudioMatrixConfig>(config);
}


ConfigParser::~ConfigParser(){
}

std::shared_ptr<AudioMatrixConfig> ConfigParser::get_config(){
    return parsed_config;
}
