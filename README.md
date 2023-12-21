# OARConv
Convert OAR to Collada for Unity

## oarconv (for Linux)
### Compile
#### needs OpenJpeg v2.5
```
git clone https://github.com/uclouvain/openjpeg.git
cd openjpeg/
mkdir build
cd build/
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make install
cd ../..
```
#### needs JunkBox_Lib
```
cd JunkBox_Lib
./config.sh
./configure --enable-jpeg --enable-openjpeg=2.5 --enable-ssl --enable-ldap
make
cd C++Lib
make
cd ../..
```
#### oarconv
```
git clone https://github.com/JunkBox-Library/oarconv.git
cd oarconv
make
./oarconv -h
```
