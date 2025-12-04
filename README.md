# Raylib test

A small learning experiece using C and [raylib](https://github.com/raysan5/raylib) to build GUI applications.

## Setup

### Prerequisites

- Meson
- Ninja
- Modern C compiler
- CMake (for building raylib)


## Build

1. Setup build directory

```shell
meson setup -C builddir
```

2. Configure compile features for LSP clients

```shell
meson configure builddir -Dauto_features=enabled
```

2. Compile programm

```shell
meson compile -C buildddir
```
