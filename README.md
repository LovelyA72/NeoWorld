# NeoWorld4U

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
3. run `cmake ..` to initialize the project
4. run `make -j$(nproc)` to compile the project
5. you should see the executable file

## Background information
The original w4u was based on an ancient version of WORLD synthesis library. In 2019, UtaForum user Zany released an updated version of w4u with the latest WORLD library a in 2019. However, the code was Windows only and requires MSVC 2010 to compile. This repository updates the project to 2021 standard by adapting the build system to CMake, and utilizing the GitHub Actions system.

## License
NeoWorld4U uses the GNU GPL v3 license to ensure a good open source environment in the UTAU community.
