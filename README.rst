LLVM Developer Meeting Tutorial: Building an Out-of-Source Pass
===============================================================

This repository contains the code supporting a tutorial concerning building,
testing and using an out-of-source LLVM project.

It assumes you properly installed LLVM **3.8**, for instance using::

    > git clone http://llvm.org/git/llvm.git --branch release_38 --depth 1 
    > cd llvm/tools
    > git clone http://llvm.org/git/clang.git --branch release_38 --depth 1
    > cd ..
    > mkdir _build && cd _build
    > cmake .. -DCMAKE_BUILD_TYPE=Releasa+Debuge -DLLVM_INSTALL_UTILS -DCMAKE_INSTALL_PREFIX=$HOME/llvm-3.8
    > make
    > make install

Which ends up installing llvm in ``$HOME/llvm-3.8``

Then to compile the tutorial code, from the source repository, where this README lies::

    > mkdir _build && cd _build
    > cmake .. -DLLVM_ROOT=$HOME/llvm-3.8
    > make


Content
-------


- The `cmake` setup is described in `CMakeLists.txt` ;

- `MBA` contains the code for a simple transformation ;

- `ReachableIntegerValues` contains the code for a simple analysis ;

- `DuplicateBB` contains the code for a slightly more complex transformation
  that relies on the above analyse ;

- `Tests` directory contains a basic lit setup ;

- `Doc` contains the slide sources.

Hopefully, most of the code is documented or self explanatory, enjoy!

NB: The code does not follow the LLVM formating guidelines, so that code
samples can be directly included in the LaTeX sources. That's a poor form of
literate programing, sorry about this.
