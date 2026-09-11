# Building from source

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -B ./build -G Ninja
cmake --build ./build --config Debug --target all
```

# Info on symlink structure for versioned install flag

## Binaries

```path
/usr/local/bin/
├── audio-matrix -> /usr/local/bin/audio-matrix-<version>
└── audio-matrix-<version>
```
