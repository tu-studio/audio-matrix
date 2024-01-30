#include <ConfigParser.h>


ConfigParser::ConfigParser(std::string config_file){
    config = YAML::LoadFile(config_file);

    std::cout << "Port: " << config["port"] << std::endl;
    std::cout << "n_input_channels: " << config["n_input_channels"] << std::endl;
    
    
    YAML::Node tracks = config["tracks"];
    if (tracks.IsDefined() && tracks.IsSequence()){
        
        std::cout << "Number of tracks: " << tracks.size() << std::endl;
        for (YAML::const_iterator it=tracks.begin();it!=tracks.end();++it){
            YAML::Node track = it->as<YAML::Node>();
            std::cout << "Parsing Track "<< track["name"] << ":" << std::endl;
            YAML::Node track_modules = track["modules"];

            if (track_modules.IsDefined() && track_modules.IsSequence()){
                for (YAML::const_iterator module_it=track_modules.begin();module_it!=track_modules.end();++module_it){
                    YAML::Node module = module_it->as<YAML::Node>();
                    std::cout << "\t" << module << std::endl; 
                }
            } else {
                std::cout << "[WARNING] Track has no Modules" << std::endl;
            }
        }
    } else {
        std::cout << "[WARNING] No tracks defined" << std::endl;
    }
    

}

ConfigParser::~ConfigParser(){
}

YAML::Node ConfigParser::get_config(){
    return config;
}
