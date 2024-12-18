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
conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
```
Then configure some more and compile
```
$ cd build
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```
If on windows instead:
```
$ cd build
# assuming Visual Studio 15 2017 is your VS version and that it matches your default profile
$ cmake .. -G "Visual Studio 15 2017" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
$ cmake --build . --config Release
```
