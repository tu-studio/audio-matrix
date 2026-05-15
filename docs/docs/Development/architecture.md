# Architecture

The `AudioMatrix` contains `Track`s, which each contain `Module`s, which do the actual audio processing.

During Construction of the `Track`s, the `AudioMatrix` passes a shared pointer to the OSC Server Thread to them, which is passed to the modules in order for them to be able to receive OSC messages.


During initalization the `AudioMatrix` initializes each `Track`. The `Track` in turn iterates over all `Module`s, each `Module` returns its number of output channels  depending on the amount of input channels it gets. The number of output channels of each `Track` is added up, so the `AudioMatrix` can report to the `JackClient` the amount of JACK ports that need to be opened.
During 

Actual buffers get reserved during the `prepare` stage, which follows the same logic as the initalization stage.

During processing the input channels are passed into each `Track`, the `Track` starts by copying the input buffers into its own `m_buffer`. then each `Module` uses its `process()` function on this buffer. At the end of the processing stage the amount of output channels earlier declared by this track is copied from the `m_buffer` into the main output buffer `out`.

