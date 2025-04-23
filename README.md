# voke

Independent lightweight cross-platform C/C++ library-manager.

Until Windows support is done, VokeGpu will not implement support for Linux, or, [if you want to contribute, check the coding guide-style](https://github.com/vokegpu/code-of-conduct-and-style-guide), thanks.

All you need: CMake, Ninja. GNU, LLVM, or a Microsoft compiler.
```sh
cmake -S . -B ./cmake-build -G Ninja
cmake --build ./cmake-build
```

## Commands

Of course `voke` is not done yet, it is on initial phase, but already include some commands.

```sh
voke -v
voke --version

voke -h
voke --help
```

## Technical Details

[Please read here the technical details](https://github.com/vokegpu/voke-docs)

@vokegpu
