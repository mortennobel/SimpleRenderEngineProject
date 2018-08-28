# SimpleRenderEngine Projects

This is a fork of SimpleRenderEngine including libraries and bindings to common game development libraries. (This includes SimpleRenderEngine dependencies)
The goal of this project is be able to get started without spending time on setup.

First clone the project (`git clone --recurse-submodules https://github.com/mortennobel/SimpleRenderEngineProject.git`). You can use of the examples as starting point.

For more information about SimpleRenderEngine see:

https://github.com/mortennobel/SimpleRenderEngine

# Setup OSX

## SDL
Download the most recent Development Libraries for macOS from https://www.libsdl.org/download-2.0.php
Open the dmg and copy the SDL2.framework to /Library/Frameworks 

## SDL_image

Download the most recent Development Libraries for macOS from https://www.libsdl.org/projects/SDL_image/
Open the dmg and copy the SDL2_image.framework to /Library/Frameworks 

## SDL_mixer

Download the most recent Development Libraries for macOS from https://www.libsdl.org/projects/SDL_mixer/
Open the dmg and copy the SDL2_mixer.framework to /Library/Frameworks 

## Run CMake to setup project 

CMake should now be able to configure your project fully using CMake with default values.

# Setup Windows (Visual Studio 2017)

Every dependency is included. Run CMake and use the default options to generate the project files.
Use x86 (64-bit is currently unsupported).

# Setup Linux

* sudo apt-get install libsdl2-dev
* sudo apt-get install libsdl2-image-dev
* sudo apt-get install libsdl2-mixer-dev
* sudo apt-get install libglew-dev
* sudo apt-get install lua5.3-dev