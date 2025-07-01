# Configuration

Configuration is done using a config file in YAML format. Some example configs can be found in `examples/configs`.

## General Configuration:

| Option Name        | Description                                                           | Default Value |
| ------------------ | --------------------------------------------------------------------- | ------------- |
| `port`             | Port the OSC-Server listens on                                        | `12345`       |
| `n_input_channels` | Amount of Input channels to be created. these are sent to every track | `64`          |
| `tracks`           | Array of `tracks`, see "Configuration of Tracks"                      | `[]`          |

## Configuration of Tracks:

| Option Name | Description                                        | Default Value |
| ----------- | -------------------------------------------------- | ------------- |
| `name`      | Name of this track                                 | `""`          |
| `modules`   | Array of `modules`, see "Configuration of Modules" | `[]`          |

## Configuration of Modules:

Modules always start with the name of the module as the outermost key. The module names and module specific options can be found [here](modules.md).

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