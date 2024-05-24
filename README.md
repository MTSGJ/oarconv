# OARConv

## OverView
* Convert OAR file of OpenSimulator to Collada/OBJ files for Unity/UE5
* **OARConv** is for Linux/Unix
* **OARConvWin** is for MS Windows

### Wiki
* [Wiki of OARConv](https://polaris.star-dust.jp/pukiwiki/?OAR+Converter)
* [Wiki of OARConvWin](https://polaris.star-dust.jp/pukiwiki/?OARConvWin)
    * [Simple Wiki of OARConvWin](https://github.com/MTSGJ/oarconv/wiki)

### Demo
#### WebGL
* http://blackjack.nsl.tuis.ac.jp/unity/TUIS_NM/  with Unity Chan
* http://blackjack.nsl.tuis.ac.jp/unity/TUIS_HW/  with Halloween Unity Chan
* http://blackjack.nsl.tuis.ac.jp/unity/TUIS_SD/ with SD Unity Chan
* http://blackjack.nsl.tuis.ac.jp/unity/TUIS_TWIN/ with Unity Chan and SD Unity Chan
#### Youtube
* [Demo Video](https://www.youtube.com/watch?v=LUhqNJHLCuM)

### Latest Version
* v1.7.x
  * [OARConvWin-1.7.2.zip for MS Windows](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConvWin-1.7.2.zip)


## OARConv (for Linux)
### Usage
#### Help
```
 oarconv [-i OAR_directory] [-o output_directory] [-a adding_assets_directories_that_separated_by_':'] [-f object_xml_file]
         [-t terrain_texture_scale] [-c external_convert_command_of_jp2]
         [-s start_no.] [-e end_no.]
         [-x shift_of_x_direction] [-y shift_of_y_direction] [-z shift_of_z_direction]
         [--dae | --obj | --stl] [--unity | --ue] [--dg | --nooffset] [--joint]
         [-d] [-v] [-h | --help]

   -i : specify OAR directory. default is ./
   -o : specify output directory. default is ./DAE/
   -f : specify object xml file. only specified file is converted.
   -a : specify adding assets directories that separated by ':'. default is /usr/local/share/oarconv/assets/:./assets/
   -t : specify Terrain texture scale. default is 7.000000
   -c : specify external convert command from jp2 to other image. default is "/usr/local/bin/opj_decompress -i %s -o %s >/dev/null 2>&1"
   -s : specify start number of xml file. default is 0.
   -e : specify end number of xml file. count from zero. default is -1 (minus number means infinity).
   -x : specify shift of x direction of all objects. default is 0.0
   -y : specify shift of y direction of all objects. default is 0.0
   -z : specify shift of z direction of all objects. default is 0.0
   --dae : output Collada DAE file(s).
   --obj : output Wavefront OBJ file(s).
   --stl : output STL ASCII file(s).
   --unity : for Unity Engine.
   --ue : for Unreal Engine.
   --dg : shrinkage to the origin with DAE/OBJ file.
   --nooffset : same for --dg
   --joint : Process Joints
   -d : debug mode. display debug information.
   -v : display version information.
   -h : display this help messages.
   --help : display this help messages.
```

#### Example
* oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets -d
* oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets --dae --unity --joint
* oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets --obj --ue --dg -d

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
./configure --enable-jpeg --enable-openjpeg=/usr/local:2.5
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
- [OARConvWin-1.7.2.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConvWin-1.7.2.zip)
- [OARConvWin-1.6.0.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConvWin-1.6.0.zip)
- [OARConvWin-1.5.0.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConvWin-1.5.0.zip)
- [OARConvWin-1.2.1.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConvWin-1.2.1.zip)
  
### Screen Shot
<img src="https://github.com/fumihax/oarconv/assets/95947474/b5779945-fa2c-48e6-a7eb-2db346d2b670" width="25%" >

