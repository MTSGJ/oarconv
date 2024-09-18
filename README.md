# OARConverter

## OverView
* Convert OAR file of OpenSimulator to Collada/OBJ/glTF(glb) files for Unity/UE5
* **OARConv** is for Linux/Unix
* **OARConvWin** is for MS Windows

### Wiki
* [Wiki of OARConv](https://polaris.star-dust.jp/pukiwiki/?OARConverter)
* [Wiki of OARConvWin](https://polaris.star-dust.jp/pukiwiki/?OARConvWin)
    * [Simple Wiki of OARConvWin](https://github.com/MTSGJ/oarconv/wiki)

### Blog
* Blog of introductions by Austin: 
   * http://blog.inf.ed.ac.uk/atate/2015/10/24/opensim-oar-convert-to-unity-scene-with-windows-interface/
   * https://blog.inf.ed.ac.uk/atate/2024/06/10/oar-converter/

### Demo
#### WebGL by Unity
- http://blackjack.nsl.tuis.ac.jp/unity/TUIS_NM/  TUIS with Unity Chan
- http://blackjack.nsl.tuis.ac.jp/unity/TUIS_HW/  with Halloween Unity Chan
- http://blackjack.nsl.tuis.ac.jp/unity/TUIS_SD/ with SD Unity Chan
- http://blackjack.nsl.tuis.ac.jp/unity/TUIS_TWIN/ with Unity Chan and SD Unity Chan
- http://blackjack.nsl.tuis.ac.jp/unity/Marineville/ with ILTan (Data from [Austin Tate](http://www.aiai.ed.ac.uk/~bat/) )
- http://blackjack.nsl.tuis.ac.jp/unity/OpenVCE/ with Naka-Chan (Data from [Austin Tate](http://www.aiai.ed.ac.uk/~bat/) )
#### Youtube
- https://youtu.be/AOnfXmVD5uM (v1.6.0)
- https://youtu.be/LUhqNJHLCuM (v1.2.0)
- https://youtu.be/eOd5Vsb85q4 (v1.0.0)
#### Videos at BlackJack
- https://blackjack.nsl.tuis.ac.jp/video/PronamaChan_are_go.mp4 (v1.7.x with Joints)
- https://blackjack.nsl.tuis.ac.jp/video/OARConvWin1.6.0_UE_Demo.mp4 (v1.6.0 for UE)

### Latest Version
* v1.8.3 (2024/09/18)
  * implemented a function to expand OAR file for Regions 

* v1.8.2 (2024/09/15)
  * add handling of cases where objects with and without joints are mixed
  * GLTFAutoCollider.cs v1.1.1 for Unity

* v1.8.0 (2024/09/06)
  * support glTF/glb file output for Unity / UE5  (DAE, OBJ and STL, too)
  * support PNG image file
  * support UE5 Import Bug for glTF/glb with skeleton.
  * fixed some bugs


## OARConverter (for Linux)
### Usage
#### Help
```
 oarconv [-i OAR_directory] [-o output_directory] [-a adding_assets_directories_that_separated_by_':'] [-f object_xml_file]
         [-t terrain_texture_scale] [-c external_convert_command_of_jp2]
         [-s start_no.] [-e end_no.]
         [-x shift_of_x_direction] [-y shift_of_y_direction] [-z shift_of_z_direction]
         [--dae | --obj | --gltf | --glb | --stl] [--unity | --ue] [--dg | --nooffset] [--joint]
         [-d] [-v] [-h | --help]
 
   -i : specify OAR directory. default is ./
   -o : specify output directory. default is DAE/
   -f : specify object xml file. only specified file is converted.
   -a : specify adding assets directories that separated by ':'. default is /usr/local/share/oarconv/assets/:./assets/
   -t : specify Terrain texture scale. default is 7.000000
   -c : specify external convert command from jp2 to other image. default is "/usr/local/bin/j2k_to_image -i "%s" -o "%s" >/dev/null 2>&1"
   -s : specify start number of xml file. default is 0.
   -e : specify end number of xml file. count from zero. default is -1 (minus number means infinity).
   -x : specify shift of x direction of all objects. default is 0.0
   -y : specify shift of y direction of all objects. default is 0.0
   -z : specify shift of z direction of all objects. default is 0.0
   --oar : specify OAR file. The -i option has priority.
   --dae : output Collada DAE file(s).
   --obj : output Wavefront OBJ file(s).
   --gltf: output glTF file(s).
   --glb : output glb file(s).
   --stl : output STL ASCII file(s).
   --unity : for Unity Engine.
   --ue  : for Unreal Engine.
   --dg  : degenerate shift data to the origin.
   --tga : use TGA file as texture.
   --png : use PNG file as texture. (default)
   --nooffset : same for --dg
   --joint : Process Joints
   -d : debug mode. display debug information.
   -v : display version information.
   -h : display this help messages.
   --help : display this help messages.
 
 ex.) oarconv -i OAR -a /usr/local/opensim/bin:./assets --dae --unity --joint --tga
 ex.) oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets --obj --ue --dg -d
 ex.) oarconv -i OAR -s 0 -e 0 --dae --unity # output terrain data only
```

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

## OARConvWin (for MS Windows)
### Download Binary
- [OARConvWin-1.8.3.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConverter/OARConvWin-1.8.3.zip)  (glTF/glb support)
- [OARConvWin-1.7.12.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConverter/OARConvWin-1.7.12.zip)
- [OARConvWin-1.6.0.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConverter/OARConvWin-1.6.0.zip)
- [OARConvWin-1.5.0.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConverter/OARConvWin-1.5.0.zip)
- [OARConvWin-1.2.1.zip](https://blackjack.nsl.tuis.ac.jp/Download/Release/OARConverter/OARConvWin-1.2.1.zip)
  
### Screen Shot
<img src="https://github.com/fumihax/oarconv/assets/95947474/b5779945-fa2c-48e6-a7eb-2db346d2b670" width="25%" >


## Addresses of thanks.
- This software uses the source code and resource of OpenSim in part. see also http://opensimulator.org/
- This software uses the source code of OpenMetaverse in part. see also  https://github.com/openmetaversefoundation
- This software uses OpenJpeg command and library. see also http://www.openjpeg.org/
- This software uses zlib library. see also http://www.zlib.net/
- This software uses tar32.dll. see also https://www.madobe.net/archiver/lib/tar32.html
- This software uses libpng. see also https://daeudaeu.com/libpng/
- Thank you very much for all contributors!!
