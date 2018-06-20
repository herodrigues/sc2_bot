# ```sc2_bot```

StarCraft II API template bot using SC2 Client API (C++). There is nothing important implemented in the bot code, it's just a template project with the s2client-api as git submodule using CMake.

## Requirements

Download an install: 

* StarCraft II ([Linux](https://github.com/Blizzard/s2client-proto#downloads), [Windows](https://www.battle.net/download/getInstallerForGame?os=win&locale=enUS&version=LIVE&gameProgram=STARCRAFT_2) or [Mac](https://www.battle.net/download/getInstallerForGame?os=mac&locale=enUS&version=LIVE&gameProgram=STARCRAFT_2))
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
$ git clone --recursive https://github.com/herodrigues/sc2_bot.git
$ cd sc2_bot
$ mkdir build 
$ cd build
$ cmake ../ -G "Visual Studio 15 2017 Win64"
$ start sc2_bot.sln
```

### Linux

```bash
$ git clone --recursive https://github.com/herodrigues/sc2_bot.git
$ cd sc2_bot
$ mkdir build && cd build

# Use 'cmake -DCMAKE_BUILD_TYPE=Debug ../' to display debug information
$ cmake ../
$ make
$ ./bin/sc2_bot -e <path-to-sc2-executable>
```
