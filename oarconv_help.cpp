/*    
*/

#include "oarconv_help.h"


void  oarconv_help(FILE* fp)
{
    fprintf(fp, "\n");
    fprintf(fp, " oarconv ");
    fprintf(fp, "[-i OAR_directory] [-o output_directory] [-a adding_assets_directories_that_separated_by_':'] [-f object_xml_file]\n");
    fprintf(fp, "         [-t terrain_texture_scale] [-c external_convert_command_of_jp2]\n");
    fprintf(fp, "         [-s start_no.] [-e end_no.]\n");
    fprintf(fp, "         [-x shift_of_x_direction] [-y shift_of_y_direction] [-z shift_of_z_direction]\n");
    fprintf(fp, "         [--dae | --obj | --stl] [--unity | --ue] [--dg | --nooffset] [--joint]\n");
    fprintf(fp, "         [-d] [-v] [-h | --help]\n");
    fprintf(fp, "\n");

    fprintf(fp, "   -i : specify OAR directory. default is %s\n", OART_DEFAULT_INP_DIR);
    fprintf(fp, "   -o : specify output directory. default is %s\n", OART_DEFAULT_DAE_DIR);
    fprintf(fp, "   -f : specify object xml file. only specified file is converted.\n");
    fprintf(fp, "   -a : specify adding assets directories that separated by ':'. default is %s\n", OART_DEFAULT_AST_DIR);
    fprintf(fp, "   -t : specify Terrain texture scale. default is %f\n", TRNT_DEFAULT_TEX_SCALE);
    fprintf(fp, "   -c : specify external convert command from jp2 to other image. default is \"%s\"\n", OART_JP2_DECOMP_COM);
    fprintf(fp, "   -s : specify start number of xml file. default is 0.\n");
    fprintf(fp, "   -e : specify end number of xml file. count from zero. default is -1 (minus number means infinity).\n");
    fprintf(fp, "   -x : specify shift of x direction of all objects. default is 0.0\n");
    fprintf(fp, "   -y : specify shift of y direction of all objects. default is 0.0\n");
    fprintf(fp, "   -z : specify shift of z direction of all objects. default is 0.0\n");

    fprintf(fp, "   --dae : output Collada DAE file(s).\n");
    fprintf(fp, "   --obj : output Wavefront OBJ file(s).\n");
    fprintf(fp, "   --stl : output STL ASCII file(s).\n");
    fprintf(fp, "   --unity : for Unity Engine.\n");
    fprintf(fp, "   --ue : for Unreal Engine.\n");
    fprintf(fp, "   --dg : shrinkage to the origin with DAE/OBJ file.\n");
    fprintf(fp, "   --nooffset : same for --dg\n");
    fprintf(fp, "   --joint : Process Joints\n");

    fprintf(fp, "   -d : debug mode. display debug information. \n");
    fprintf(fp, "   -v : display version information. \n");
    fprintf(fp, "   -h : display this help messages. \n");
    fprintf(fp, "   --help : display this help messages. \n");
    fprintf(fp, "\n");
    fprintf(fp, " ex.) oarconv -i OAR -a /usr/local/opensim/bin:./assets --dae --unity --joint \n");
    fprintf(fp, " ex.) oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets --obj --ue --dg -d\n");
    fprintf(fp, " ex.) oarconv -i OAR -s 0 -e 0 --dae --unity # output terrain data only\n");
    fprintf(fp, "\n");

    fflush(fp);
    return;
}

