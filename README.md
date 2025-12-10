# Raylib test

A small learning experiece using C and [raylib](https://github.com/raysan5/raylib) to build GUI applications.

## Setup

### Prerequisites

- Meson
- Ninja
- Modern C compiler
- CMake (for building raylib)

### WSL Ubuntu dependencies
```shell
sudo apt update
sudo apt install libxinerama-dev libxrandr-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

## Build

1. Setup build directory

```shell
meson setup builddir
```

2. (Optional) Configure compile features for LSP clients

```shell
meson configure builddir -Dauto_features=enabled
```

3. Compile programm

```shell
meson compile -C buildddir
```
