# DeepAxie v1.0

DeepAxie is a simplified version of the NFT-card-game Axie Infinity

The following picture is how the environment looks when executing in 
python.
![](https://github.com/cair/DeepAxie/blob/main/environment.png)


## Reinforcement Learning using DQN

To use the environment in python, check the file test.py.

## To compile the library
Compile lib on Mac (x86)
```
clang -O3 -Wall -shared -undefined dynamic_lookup -std=c++17 -fPIC 
$(python3 -m pybind11 --includes) DeepAxie.cpp -o DeepAxie.so -target 
x86_64-apple-darwin-macho
```

Compile lib on Mac (M1)
```
clang -O3 -Wall -shared -undefined dynamic_lookup -std=c++17 -fPIC 
$(python3 -m pybind11 --includes) DeepAxie.cpp -o DeepAxie.so -target 
arm64e-apple-darwin-macho
```

Compile lib on linux:
```
g++ -O3 -Wall -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) 
DeepAxie.cpp -o DeepAxieLinux.so
```


To compile the library for windows you might want to use `"g++"` or 
`"gcc"`.

To run code with library on M1 mac, try the following link:
https://github.com/apple/tensorflow_macos


For compiling library on different operating systems, check "Target 
Triple" from this link:
https://clang.llvm.org/docs/CrossCompilation.html


## conpile library with pybind11 inside Docker

Python & C++

This was made as a workaround when developing on different systems, but 
works across systems by using Docker!

How to use:
```
cd pybind11-docker
cd base
docker build . -t base
cd ../app
docker build . -t app
docker run --rm -it app
```

when inside the container:

```
python3
import test
test.add(1, 5)
```
or
```
python3 hello.py
```

Licence
Copyright 2022 Rune Alexander Laursen, Peshang Alo

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.

The implementation is inspired by: 
https://medium.com/@keithwhitley/using-c-with-python-3-in-2018-480f3e46c8c 
and https://youtu.be/R9dtxqVdc8M.
