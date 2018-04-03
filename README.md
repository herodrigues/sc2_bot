# ```basileus```

StarCraft II API bot.

## Requirements

Download an install: 

* [Ladder 2017 Season 3 map pack](http://blzdistsc2-a.akamaihd.net/MapPacks/Ladder2017Season3.zip)
* [CMake](https://cmake.org/download/)

#### Linux
* ```make``` library.

#### OS X
* XCode command line tools.

#### Windows
* [Visual Studio 2017](https://www.visualstudio.com/downloads/)

## Installation

### Windows

```bash
$ git clone --recursive https://github.com/herodrigues/basileus.git
$ cd basileus
$ mkdir build 
$ cd build
$ cmake ../ -G "Visual Studio 15 2017 Win64"
$ start basileus.sln
```

### Linux

```bash
$ git clone --recursive https://github.com/herodrigues/basileus.git
$ cd basileus
$ mkdir build && cd build

# Use 'cmake -DCMAKE_BUILD_TYPE=Debug ../' to display debug information
$ cmake ../
$ make
$ ./bin/basileus -e <path-to-sc2-executable>
```
