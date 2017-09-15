# giygas [![Build Status](https://travis-ci.org/sfuller/giygas.svg?branch=master)](https://travis-ci.org/sfuller/giygas)
Because I'm an almighty idiot (and I ran out of names for my projects)

## What
Giygas is a framework for games and other graphic applications. It provides a modern c++ interface to graphics apis and provides a few extra useful utilities as well.

## How do I build it?
Wow, sold already? Here's how building giygas will go down:

### Use 'dew' to download dependencies
I built a cute little tool called dew to download, compile, and install dependencies to an isolated prefix directory.

**Note:** If you don't know what generator name to use, run `cmake -G` to see a list of available generators.

```shell
# Run these commands in the source directory
# Python3 must be installed
pip3 install dew-pacman
python3 -m dew --cmake-generator "Generator Name"
```

Awesome, hopefully that worked. If it did, you should now have the directory `.dew/install` in your source directory and the `install` directory should have all of the precious dependencies in it. That directory is the prefix path that you can point CMake to when building

**Note:** You can skip using my tool if you install the dependencies with your package manager. Just make sure you omit passing the prefix variables to cmake in the next steps.

### Build with CMake

```
# Run these commands in a out-of-source build directory.
# Usually this is a new directory inside of the source directory. e.g.: mkdir build
# Also, ${SOURCE_DIR} refers to giygas's source directory, 
# which is probably .. if you followed the steps above
cmake -G "Generator Name" ${SOURCE_DIR} -DCMAKE_PREFIX_PATH="${SOURCE_DIR}/.dew/install" -DCMAKE_INSTALL_PREFIX="${SOURCE_DIR}/.dew/install"
cmake --build . --target install
```

## Great, I built it, but how do I use it?
The steps above should have installed a built giygas library into the `.dew/install` directory. You can use that directory as a prefix in your buildsystem. You could also use my dew tool and use giygas as a dependency. See the example projects (comming soon) for a demonstration of how to achieve this.

## This thing is broken. Can you fix it?
Sure can, or, I can at least try. Don't be shy about creating github issues. Even if they're just questions.

## Can I fix it?
You sure can, or, you can at least try. Don't be shy about creating pull requests. Even if they're dumb.

