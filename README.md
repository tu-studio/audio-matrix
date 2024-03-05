# Building
``` bash
cmake -DCMAKE_BUILD_TYPE=Debug -B ./build
cmake --build ./build --config Debug --target all 
```

# Configuration
TODO

# Adding a new module
To add a module, several files have to be edited, until a more streamlined approach is found.

## 1. Add identifier of Module to Modules enum
In `ModuleConfig.h` add an identifier of your module to the `Modules` enum


## 2. Create Module Files
Modules must inherit from `Module`.

Modules need a config struct inheriting from `ModuleConfig`.

Modules should be created in a subfolder of Module following this directory structure:

```
source
└── Module
    ├── MyModule
    │   ├── MyModuleConfig.h
    │   ├── MyModule.cpp
    │   └── MyModule.h
    ├── ModuleConfig.h
    ├── Module.cpp
    └── Module.h
```
1. Create your classes

TODO explain classes


## 3. Parsing of Module Config
Parsing of the config happens in the `ConfigParser`, so we need to add the module-specific config parser there.

Add an include directive for your config in `ConfigParser.h`:
```c++
#include <MyModuleConfig.h>
```

Add a function signature for the parser function to `ConfigParser.h`, next to the other similar functions:
```c++
ModuleConfigPtr parse_module_mymodule(YAML::Node module);
```


Implement this function in `ConfigParser.cpp`. Parsing is done using the [yaml-cpp](https://github.com/jbeder/yaml-cpp) library.


The call to `parse_module_mymodule` should happen in `ConfigParser::parse_module()`, so you have to add an if-case for your module to the case structure there:
```cpp
} else if (name == "mymodule") {
    return parse_module_mymodule(module);
}
```

## 4. Adding Module to Initialization
Modules are initialized by the `Track` they belong to, so you have to add a case for your module in the Constructor of `Track` in the file `Track/Track.cpp`:

```cpp
case Modules::GAIN:
{
    auto mymodule_config = std::dynamic_pointer_cast<MyModuleConfig>(module_config);
    std::shared_ptr<MyModule> mymodule = std::make_shared<MyModule>(mymodule_config, osc_server);
    m_modules.push_back(std::static_pointer_cast<Module>(mymodule));
}
break;
```
