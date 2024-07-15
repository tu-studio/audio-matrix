# Audio-Matrix
The Audio Matrix is a configurable router and signal processor for Audio-Setups with lots of channels/speakers that offers built in remote control via OSC.

A configurable amount of input tracks is routed in parallel through user configurable tracks, that can process the audio on them using a variety of modules.

An example Procesing setup could look like this:
![An Image of the DSP flow in the audio matrix consisting of 3 Tracks, that each contain a number of modules](assets/AudioMatrix.png)

# Building
``` bash
cmake -DCMAKE_BUILD_TYPE=Debug -B ./build -G Ninja
cmake --build ./build --config Debug --target all 
```

# Configuration
Configuration is done using a config file in YAML format. Some example configs can be found in `examples/configs`.

## General Configuration:

| Option Name | Description | Default Value |
| --- | ---| ---| 
| `port` | Port the OSC-Server listens on | `12345` |
| `n_input_channels` | Amount of Input channels to be created. these are sent to every track | `64` |
| `tracks` | Array of `tracks`, see "Configuration of Tracks"  | `[]` |

## Configuration of Tracks:

| Option Name | Description | Default Value |  
| --- | --- | --- |
| `name` | Name of this track | `""` |
| `modules` | Array of `modules`, see "Configuration of Modules" | `[]` |

## Configuration of Modules:
Modules always start with the name of the module as the outermost key. The module names can be found in the following "Module: ..." sections.

For simple modules that don't take any parameters like `sum` only the name is needed, you could specify it as
```yaml
tracks:
  - name: example track
    modules: 
      - sum
```
for more complex modules additional options are needed. If the module should be OSC-controllable, the option `osc_path` is required. it expects as a string the osc path this module should listen to. complex modules might use this path as a base path to listen on different subpaths, see their individual configs for that.

Some options called "primary options" can be directly set as the value of a module type, for example

```yaml
tracks:
  - name: example track
    modules: 
      - gain: 0.5
```
This only works if this is the only option that is manually set.

In all other cases modules are defined using more key-value-pairs:
```yaml
tracks:
  - name: example track
    modules: 
      - hoa_encoder:
        order: 3
        osc_path: /positions
```


### Module: Gain


|   |   |
|---|---|
| name |`gain`|
|OSC controllable | yes |
| OSC format |  `[osc_path] if <channel_index> <channel_gain>` |

Module used to apply individual gains per channel.
OSC control enables enables the setting of these gains.

| Option Name | Description | Default Value |  
| --- | --- | --- |
| `factor` | Initial gain for all channels | `1` |



### Module: Ambisonics Encoder
|   |   |
|---|---|
| name |`hoa_encoder`|
|OSC controllable | yes |
| OSC format |  `[osc_path] ifff <channel_index> <azimuth_in_rad> <elevation_in_rad> <distance_in_m>` |

This module encodes the incoming audio into the ambisonics domain. channels are ordered following the AmbiX format. The individual positions for each channel are settable using OSC.

| Option Name | Description | Default Value |  
| --- | --- | --- |
| `order` | Ambisonics order to encode to, currently orders up to 4 are supported | `3` |

### Module: Sum
|   |   |
|---|---|
| name |`sum`|
|OSC controllable | no |

This module sums up all incoming channels into one channel

### Module: Filter
|   |   |
|---|---|
| name |`filter`|
|OSC controllable | no |

This module implements Linkwitz-Riley lowpass- or highpass filters.

| Option Name | Description | Default Value |  
| --- | --- | --- |
| `order` | Order of the Linkwitz-Riley Filter, order of the individual butterworth filters will be `order/2` | `4` |
| `freq` | Cutoff Frequency of the Filter (in Hz) | `150` |
| `type` | Filter type, options are `LP` and `HP` | `HP`|

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
Modules are initialized by the `Track` they belong to, so you have to add a case for your module in the Constructor of `Track` in the file `Track/Track.cpp`:

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
# Licenses
This Project is licensed under the GPLv3.

Calculation of the IIR filter coefficients is performed using the IIR library by [Exstrom Laboratories](https://www.exstrom.com/journal/sigproc/dsigproc.html), also licensed under the GPLv3