# Configuration

Configuration is done using a config file in YAML format. Some example configs
can be found in `examples/configs`.

## General Configuration:

| Option Name        | Description                                                                                  | Default Value |
| ------------------ | -------------------------------------------------------------------------------------------- | ------------- |
| `port`             | Port the OSC-Server listens on.                                                              | `12345`       |
| `n_input_channels` | Amount of input channels to be created. These are sent to every track.                       | `64`          |
| `tracks`           | Array of `tracks`, see [here](configuration.md#configuration-of-tracks).                     | `[]`          |

## Configuration of Tracks:

| Option Name | Description                                                                | Default Value |
| ----------- | -------------------------------------------------------------------------- | ------------- |
| `name`      | Name of this track.                                                        | `""`          |
| `modules`   | Array of `modules`, see [here](configuration.md#configuration-of-modules). | `[]`          |



## Configuration of Modules:

Modules always start with the name of the module as the outermost key. The
module names and module specific options can be found [here](modules.md).


For simple modules that don't take any parameters (like `sum`) only the name is
needed:

```yaml
tracks:
  - name: example track
    modules:
      - sum
```

Primary options can be directly set as the value of a module type. This only
works if this is the only option that is manually set:

```yaml
tracks:
  - name: example track
    modules:
      - gain: 0.5
```

More complex modules require additional options. If the module should be
controlled via OSC, the option `osc_path` is required. It expects the osc path
this module should listen to. Complex modules might use this path as a base
path to listen on different subpaths, see their individual configs for that. In
all other cases modules are defined using more key-value-pairs:

```yaml
tracks:
  - name: example track
    modules:
      - hoa_encoder:
          order: 3
          osc_path: /positions
```
