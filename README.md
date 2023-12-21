# OARConv
Convert OAR to Collada for Unity

## oarconv (for Linux)
### Usage

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

 ex.) oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets -d
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
