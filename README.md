# voke

Independent lightweight cross-platform C/C++ library-manager.

`voke` works with syncing libraries and compilers from the [voke-repositories](https://github.com/voke-libraries), where all compilers and libraries exists mapped, mapped with voke-files, `.voke`-file describe fixed arguments to map library per compiler target/or compiler, but do not worry, it is mainted by @VokeGpu.

The plans for now is focus on Windows support, Linux is not the priority (but it works!).

Welcome ><, if you want to contribute check the [Vokegpu standard](https://github.com/vokegpu/standard), thanks.

All you need to compile: CMake, Ninja. GNU, LLVM, or a Microsoft compiler.
```sh
cmake -S . -B ./cmake-build -G Ninja
cmake --build ./cmake-build
```

No C++ library is needed, just external dependencies tools like CMake, Ninja, Git. Soon `voke` must support Ninja, CMake sync --- as a part of fundementals compilation support.

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

  voke | -sat                  sync all C/C++ compilers-installed in voke-system 
       | --sync-all-targets
       | -sal                  sync all C/C++ libraries-installed in voke-system 
       | --sync-all-libraries

  voke | -v                    output software version
       | --version
```

### Libraries

When syncing a library, you can specify which compiler target you want.

For example sync [SDL](https://www.libsdl.org/) with your [Clang-Mingw64](https://github.com/mstorsjo/llvm-mingw) on Windows:
`voke -s libsdl2 -c clang-mingw64`.

### Compilers

When using `voke`, there is a standard format to use, it is a cross-platform library manager, you need to specifiy which C/C++ target compiler you want to use.  
For x86_64 compilers, tag suffix MUST be `64`, as example: `clang64` `clang-msvc64`. For x86 compilers, suffix MUST be `32`, as example: `clang32` `clang-msvc32`.  

| OS | Tag |
| - | - |
| Windows | `clang-msvc64` `clang-msvc32` `clang-mingw64` `clang-mingw64` `mingw64` `mingw32` |
| Linux  | `clang64`, `clang32`, `gnu64` `gnu32` |

A compiler format must be like this because of how `voke` works, `voke` serialize everything in a way to support cross-platform.

## Technical Details

[Please read here the technical details](https://github.com/vokegpu/voke-docs)

@vokegpu
