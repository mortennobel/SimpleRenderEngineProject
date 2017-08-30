# SimpleRenderEngine Projects

This is a fork of SimpleRenderEngine including libraries and bindings to common game development libraries. (This includes SimpleRenderEngine dependencies)
The goal of this project is be able to get started without spending time on setup.

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

# Libraries included

* SDL2  2.0.5 - Cross-platform development library (https://www.libsdl.org)
* SDL2 image  2.0.1 - Image loading library (https://www.libsdl.org/projects/SDL_image/)
* SDL2 mixer  2.0.1 - Sample multi-channel audio mixer library (https://www.libsdl.org/projects/SDL_mixer/)
* Glew 2.0.0 - The OpenGL Extension Wrangler Library (https://sourceforge.net/projects/glew/files/glew/2.1.0/)
* GLM 0.9.8.5 - OpenGL Mathematics (https://glm.g-truc.net)
* ImGUI 1.51 - Immediate Mode Graphical User interface (https://github.com/ocornut/imgui)
* PicoJSON 1.3.0 -JSON parser serializer (https://github.com/kazuho/picojson)
* Sol2 2.18.1 - Lua API wrapper (https://github.com/ThePhD/sol2)
* Box2D 2.3.1 - 2D physics engine for games (http://box2d.org)

# Project structure

* cmake - contains cmake package information
* examples - SimpleRenderEngine examples
* include - SimpleRenderEngine include folder
* project - SimpleRenderEngine examples using other game development libraries (e.g. Box2D, Lua)
* src - SimpleRenderEngine source
* thirdparty - other game development libraries and headers (e.g. Box2D, Lua)