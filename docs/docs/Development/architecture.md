# Architecture

The `AudioMatrix` contains `Track`s, which each contain `Module`s, which do the
actual audio processing.

During construction of the `Track`s, the `AudioMatrix` passes a shared pointer
to the OSC server thread to each `Track`. This pointer is in turn passed to the
modules in order for them to be able to receive OSC messages.

During initalization, the `AudioMatrix` initializes each `Track`. The `Track`
in turn iterates over all `Module`s, each `Module` returns its number of output
channels  depending on the amount of input channels it gets. The number of
output channels of each `Track` is added up, so the `AudioMatrix` can report
the amount of JACK ports that need to be opened to the `JackClient`

Actual buffers get reserved during the `prepare` stage, which follows the same
logic as the initalization stage.

During processing, the input channels are passed into each `Track`. The `Track`
starts by copying the input buffers into its own `m_buffer`. Then, each
`Module` uses its `process()` function on this buffer. At the end of the
processing stage the amount of output channels earlier declared by this track
are copied from the `m_buffer` into the main output buffer `out`.

