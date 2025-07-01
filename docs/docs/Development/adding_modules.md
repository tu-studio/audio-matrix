
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

Implement this function in `ConfigParser.cpp`. Parsing is done using the [yaml-cpp](https://github.com/jbeder/yaml-cpp) library. For simple key/value pairs in the config you can use the `get_config_option()` function for convenience. If your Module is controlled using OSC, call `parse_module_osc_params()` for your module.

The call to `parse_module_mymodule` should happen in `ConfigParser::parse_module()`, so you have to add an if-case for your module to the case structure there:

```cpp
} else if (name == "mymodule") {
    return parse_module_mymodule(module);
}
```

## 4. Adding Module to Initialization

Modules are initialized by the `Track` they belong to.
First you have to include your module header in the file `Track/Track.h` like this: `#include <MyModule.h>`, then you have to add a case for your module in the Constructor of `Track` in the file `Track/Track.cpp`:

```cpp
switch (module_config->module_type())
        {
            // MODULE SWITCH CASES GO HERE
            ...
            CASE_MODULE(Modules::MY_MODULE, MyModuleConfig, MyModule);
            ...
            // END MODULE SWITCH CASES
        default:
            break;
        }
```