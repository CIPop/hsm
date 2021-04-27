# Hierarchical Finite State Machine implementations

This repo contains experimental Hierarchical Finite State Machine implementations as described in http://www.wisdom.weizmann.ac.il/~dharel/SCANNED.PAPERS/Statecharts.pdf

## Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Run Tests

After building, from the `.\build` folder run:

```
ctest -C Debug
```
