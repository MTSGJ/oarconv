# OARConv

## OverView
* Convert OAR file of OpenSimulator to Collada files for Unity 2022.
* Please use Unity 2022.3.16f1 LTS
* There are some bugs in Unity 2023.2.3f1
* **OARConv** is for Linux/Unix
* **OARConvWin** is for MS Windows

### Wiki
* [Wiki of OARConv](https://polaris.star-dust.jp/pukiwiki/?OAR+Converter)
* [Wiki of OARConvWin](https://polaris.star-dust.jp/pukiwiki/?OARConvWin)

### Demo
* [Demo Video](https://www.youtube.com/watch?v=LUhqNJHLCuM)

### Latest Version
* OARConv : 1.4.6
* OARConvWin : 1.2.1.1


## OARConv (for Linux)
### Usage
#### Help
```
 oarconv [-i OAR directory] [-o output directory] [-a adding assets directories that separated by ':'] [-f object xml file]
         [-t terrain texture scale] [-c external convert command of jp2]
         [-s start no.] [-e end no.]
         [-x shift of x direction] [-y shift of y direction] [-z shift of z direction]
         [-b] [-p] [-d] [-v] [-h]

   -i : specify OAR directory. default is ./
   -o : specify output directory. default is ./DAE/
   -f : specify object xml file. only specified file is converted.
   -a : specify adding assets directories that separated by ':'. default is /usr/local/share/oarconv/assets/:./assets/
   -t : specify terrain texture scale. default is 7.000000
   -c : specify external convert command from jp2 to other image. default is "/usr/local/bin/opj_decompress -i %s -o %s >/dev/null 2>&1"
   -s : specify start number of xml file. default is 0.
   -e : specify end number of xml file. default is -1 (minus number means infinity).
   -x : specify shift of x direction of all objects. default is 0.0
   -y : specify shift of y direction of all objects. default is 0.0
   -z : specify shift of z direction of all objects. default is 0.0
   -b : output STL file(s) using BREP.
   -p : when linked objects include phantom even one, saved to Phantom directory.
   -d : debug mode. display debug information.
   -v : display version information.
   -h : display this help messages.
```
#### Example
* oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets -d

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
#### needs [JunkBox_Lib](https://github.com/JunkBox-Library/JunkBox_Lib)
```
git clone https://github.com/JunkBox-Library/JunkBox_Lib.git
cd JunkBox_Lib
./config.sh
./configure --enable-jpeg --enable-openjpeg=2.5 --enable-ssl --enable-ldap             # before v1.8.x
./configure --enable-jpeg --enable-openjpeg=/usr/local:2.5 --enable-ssl --enable-ldap  # after v1.9.x
make
cd C++Lib
make
cd ../..
```
#### OARConv
```
git clone https://github.com/JunkBox-Library/oarconv.git
cd oarconv
make
./oarconv -h
```
-----------
## OARConvWin (for MS Windows)
### Download Binary
- [OARConvWin-1.2.1.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConvWin-1.2.1.zip)

