# voke

Independent lightweight cross-platform C/C++ library-manager.

On initial phase, `voke` has plans to implement support for Windows, until focus on Linux, [if you want to contribute check the coding guide-style here](https://github.com/vokegpu/code-of-conduct-and-style-guide), thanks.

All you need for compile: CMake, Ninja. GNU, LLVM, or a Microsoft compiler.
```sh
cmake -S . -B ./cmake-build -G Ninja
cmake --build ./cmake-build
```

Soon `voke` must bootstrap it-self for installs dependencies, while no pre-compiled binary is released, no bootstrap is possible.

## Commands

Of course `voke` is not done yet, it is on initial phase, but already include some commands.

Note: You must have `git` installed, [visit here for download](https://git-scm.com/).

```sh
voke -v
voke --version

voke -h
voke --help
```

## Technical Details

[Please read here the technical details](https://github.com/vokegpu/voke-docs)

@vokegpu
