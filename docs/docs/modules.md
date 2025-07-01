# Modules

## Module: Gain

|                  |                                                |
| ---------------- | ---------------------------------------------- |
| name             | `gain`                                         |
| OSC controllable | yes                                            |
| OSC format       | `[osc_path] if <channel_index> <channel_gain>` |

Module used to apply individual gains per channel.
OSC control enables the setting of these gains.

| Option Name               | Description                   | Default Value |
| ------------------------- | ----------------------------- | ------------- |
| `factor` (primary option) | Initial gain for all channels | `1`           |

## Module: Ambisonics Encoder

|                  |                                                                                       |
| ---------------- | ------------------------------------------------------------------------------------- |
| name             | `hoa_encoder`                                                                         |
| OSC controllable | yes                                                                                   |
| OSC format       | `[osc_path] ifff <channel_index> <azimuth_in_rad> <elevation_in_rad> <distance_in_m>` |

This module encodes the incoming audio into the ambisonics domain. channels are ordered following the AmbiX format. The individual positions for each channel are settable using OSC.

| Option Name | Description                                                           | Default Value |
| ----------- | --------------------------------------------------------------------- | ------------- |
| `order`     | Ambisonics order to encode to, currently orders up to 4 are supported | `3`           |

## Module: Sum

|                  |       |
| ---------------- | ----- |
| name             | `sum` |
| OSC controllable | no    |

This module sums up all incoming channels into one channel

## Module: Filter

|                  |          |
| ---------------- | -------- |
| name             | `filter` |
| OSC controllable | no       |

This module implements Linkwitz-Riley lowpass- or highpass filters.

| Option Name | Description                                                                                       | Default Value |
| ----------- | ------------------------------------------------------------------------------------------------- | ------------- |
| `order`     | Order of the Linkwitz-Riley Filter, order of the individual butterworth filters will be `order/2` | `4`           |
| `freq`      | Cutoff Frequency of the Filter (in Hz)                                                            | `150`         |
| `type`      | Filter type, options are `LP` and `HP`                                                            | `HP`          |

## Module: Distance Gain

|                  |                                                 |
| ---------------- | ----------------------------------------------- |
| name             | `distance_gain`                                 |
| OSC controllable | yes                                             |
| OSC format       | `[osc_path] if <channel_index> <distance_in_m>` |

This Module adjusts the gain of a channel depending on its distance from the listener

## Module: Delay

|                  |         |
| ---------------- | ------- |
| name             | `delay` |
| OSC controllable | no      |

Simple non-interpolating delay line to adjust for latencies between different playback systems

| Option Name             | Description      | Default Value |
| ----------------------- | ---------------- | ------------- |
| `time` (primary option) | Delay time in ms | `5`           |
