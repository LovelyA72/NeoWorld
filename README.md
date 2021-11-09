# NeoWorld4U
A fork of Zany's W4U updated for building with cmake

| Platform | Build Status |
| ----------- | ----------- |
| ubuntu | ![ubuntu-status](https://github.com/LovelyA72/NeoWorld/actions/workflows/ubuntu-build.yml/badge.svg) |
| Windows | WIP | 

## System requirement
* CMake 3.2
* GNU GCC 9.3.0
* libfftw3-dev 3.3.8

## Compiling
1. git clone or download the source code as a zip archive
2. create a directory called build by `mkdir build && cd build`
3. run `cmake ..` to initalize the project
4. run `make -j$(nproc)` to compile the project
5. you should see the executable file

## License
NeoWorld4U uses the GNU GPL v3 license to ensure a good open source enviourment in the UTAU community.
