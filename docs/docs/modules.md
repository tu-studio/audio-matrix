### Module: Gain

|                  |                                        |
| ---------------- | -------------------------------------- |
| name             | `gain`                                 |
| OSC controllable | yes                                    |
| OSC format       | `[osc_path] iff (channel_index, gain)` |

Apply per-channel gain. Individual gains can be set via OSC.

| Option Name               | Description                    | Default Value |
| ------------------------- | ------------------------------  | ------------- |
| `factor` (primary option) | Initial gain for all channels.  | `1`           |

### Module: Ambisonics Encoder

|                  |                                                                                    |
| ---------------- | ---------------------------------------------------------------------------------- |
| name             | `hoa_encoder`                                                                      |
| OSC controllable | yes                                                                                |
| OSC format       | `[osc_path] ifff (channel_index, azimuth_in_rad, elevation_in_rad, distance_in_m)` |

This module encodes the incoming audio into the Ambisonics domain. Channels are
ordered following the AmbiX format. Individual positions are settable using
OSC.

| Option Name | Description                                               | Default Value |
| ----------- | --------------------------------------------------------  | ------------- |
| `order`     | Ambisonics order to encode to, currently up to order `4`. | `3`           |

### Module: Sum

|                  |       |
| ---------------- | ----- |
| name             | `sum` |
| OSC controllable | no    |

This module sums up all incoming channels into one channel.

### Module: Filter

|                  |                                                               |
| ---------------- | ------------------------------------------------------------- |
| name             | `filter`                                                      |
| OSC controllable | yes                                                           |
| OSC format       | `[osc_path] ifsi (index, frequency_in_hz, lp_or_hp, enabled)` |


This module implements Linkwitz-Riley lowpass or highpass filters.

| Option Name   | Description                             | Default Value |
| ------------- | --------------------------------------- | ------------- |
| `order`       | Order of the Linkwitz-Riley filter.     | `4`           |
| `freq`        | Cutoff frequency (in Hz).               | `150`         |
| `type`        | Filter type, options are `LP` and `HP`. | `HP`          |

### Module: Distance Gain

|                  |                                                 |
| ---------------- | ----------------------------------------------- |
| name             | `distance_gain`                                 |
| OSC controllable | yes                                             |
| OSC format       | `[osc_path] if <channel_index> <distance_in_m>` |

Apply per-channel gain depending on its distance from the listener. Individual
gains can be set via OSC.

### Module: Delay

|                  |                                               |
| ---------------- | --------------------------------------------- |
| name             | `delay`                                       |
| OSC controllable | yes                                           |
| OSC format       | `[osc_path] ifi (index, time_in_ms, enabled)` |

Simple non-interpolating delay line to adjust for latencies between different
playback systems. Individual delays are settable using OSC.

| Option Name             | Description       | Default Value |
| ----------------------- | ----------------  | ------------- |
| `time` (primary option) | Delay time in ms. | `5`           |

