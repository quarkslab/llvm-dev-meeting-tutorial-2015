LLVM Developer Meeting Tutorial: Building an Out-of-Source Pass
===============================================================

This repository contains the code supporting a tutorial concerning building,
testing and using an out-of-source LLVM project.

- The `cmake` setup is described in `CMakeLists.txt` ;

- `MBA` contains the code for a simple transformation ;

- `ReachableIntegerValues` contains the code for a simple analysis ;

- `DuplicateBB` contains the code for a slightly more complex transformation
  that relies on the above analyse ;

- `Tests` directory contains a basic lit setup.

Hopefully, most of the code is documented or self explanatory, enjoy!

NB: The code does not follow the LLVM formating guidelines, so that code
samples can be directly included in the LaTeX sources. That's a poor form of
literate programing, sorry about this.
