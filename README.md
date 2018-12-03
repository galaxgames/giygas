# giygas [![Build Status](https://travis-ci.org/galaxgames/giygas.svg?branch=master)](https://travis-ci.org/galaxgames/giygas)
The game graphics framework for almighty idiots.

## What
Giygas is a minimal and low-level graphics framework for games and other graphic applications. It provides a common, 
modern c++ interface for hardware accelerated graphics and provides a few extra useful utilities as well.

## What else?
* Utilizes cool C++11 features like move semantics
* Right now there's only an ~~OpenGL~~ Vulkan renderer implementation, but hopefully later it'll support ~~Vulkan~~, 
  DirectX and Metal!
* Comes with game loop and windowing support out of the box! Or you can write your own. It's modular! 
* Um....

## How do I build it?
Wow, sold already? Here's how building giygas will go down:

### Build Dependencies
You will need the following tools:
* CMake >3.2 (Higher recommended)
* Python 3.6 *
* Vulkan SDK (See below)

\* (Not required, but dependencies will not be able to be retrieved automatically without Python 3.6. See below for what
  those dependencies are.)

### Vulkan SDK Setup
#### Linux
* Should be available from your package manager. On Ubuntu the Vulkan SDK can be installed with the 'libvulkan-dev' package.

#### Windows
* Download and install the Vulkan SDK from LunarG: https://www.lunarg.com/vulkan-sdk/

#### macOS
* MoltenVK is supported. This is also available in LunarG's Vulkan SDK. On macOS, You may build and install the Vulkan 
Headers, Vulkan Loaders, Vulkan Validation Layers, and Molten VK manually if you wish.

### Automatically Installed Dependencies
There are more dependencies which are managed by the CMake project. These will be downloaded, built, and installed into
your build directory automatically. If you do not have Python3.6, you will have to install these dependencies 
manually. This is not recommended or supported. See 'dewfile.json' for a listing of these dependencies.

### Build with CMake

```shell
# Run these commands in a out-of-source build directory.
# Usually this is a new directory inside of the source directory.
# Also, ${SOURCE_DIR} refers to giygas's source directory, 
# which is probably .. if you followed the steps above
cmake -G "Generator Name" "${SOURCE_DIR}"
cmake --build .
```

### Installing
You can easilly install giygas into your global prefix (usually `/usr/local`) by running 
`cmake --build . --target install` in the build directory. However, it is recommended you use a different prefix to be
used by your dependency projects. The prefix can be changed by passing `-DCMAKE_INSTALL_PREFIX=/your/prefix/path` into 
the first cmake invocation listed in the build section.


## This thing is broken. Can you fix it?
Sure! Don't be shy about creating github issues. Even if they're feature requests, suggestions, or just questions!

## Can I fix it?
I don't know, can you? Please feel free to submit pull requests!
