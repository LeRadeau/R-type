# R-Type

## Build

First intall conan :
```
pip install conan
```
Then configure conan :
```
conan profile detect
```
Then configure the build :
```
mkdir build
conan install . --output-folder=build --build=missing
```
Then configure some more and compile
```
$ cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```
