# voke

Independent lightweight cross-platform C/C++ library-manager.

On initial phase, `voke` has plans to implement support for Windows, until focus on Linux, [if you want to contribute check the coding guide-style here](https://github.com/vokegpu/code-of-conduct-and-style-guide), thanks.

All you need to compile: CMake, Ninja. GNU, LLVM, or a Microsoft compiler.
```sh
cmake -S . -B ./cmake-build -G Ninja
cmake --build ./cmake-build
```

Soon `voke` must bootstrap it-self for installs dependencies, while no pre-compiled binary is released, no bootstrap is possible.

## Commands

Note: You must have `git` installed, [visit here for download](https://git-scm.com/).

```sh
use:
  voke | -s *                  sync a C/C++ library or a compiler
       | --sync *
        | -v *                 optional: version to look-up
        | --version *
        | -b                   optional: pre-compiled binary
        | --binary
        | -c                   optional: specify compilers to sync the library, e.g: clang-msvc64 clang-mingw64 etc 
        | --compilers

  voke | -r *                  remove a C/C++ library or a compiler from the system
       | --remove *

  voke | -sac                  sync all C/C++ compilers-installed in voke-system 
       | --sync-all-compilers
       | -sal                  sync all C/C++ libraries-installed in voke-system 
       | --sync-all-libraries

  voke | -v                    output software version
       | --version
```

## Technical Details

[Please read here the technical details](https://github.com/vokegpu/voke-docs)

@vokegpu
