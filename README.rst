LLVM Developer Meeting Tutorial: Building an Out-of-Source Pass
===============================================================

This repository contains the code supporting a tutorial concerning building,
testing and using an out-of-source LLVM project.

- The `cmake` setup is described in `CMakeLists.txt` ;

- `MBA` contains the code for a simple transformation ;

- `ReachableIntegerValues` contains the code for a simple analysis ;

- `DuplicateBB` contains the code for a slightly more complex transformation
  that reliess on the above analyse ;

- `Tests` contains a baisc lit setup.

Hopefully, most of the code is documented or self explinatory, enjoy!
