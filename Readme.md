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
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cd ..
cmake --build ./build ; mv build/client/r-type_client . ; mv build/server/r-type_server .
```
Now launch !
```
# start the server :
./r-type_server
# start the client(s) :
./r-type_client <ip> <username>
```
