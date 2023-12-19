/*    
*/


#include "oarconv_help.h"



void  oarconv_help(FILE* fp)
{
    fprintf(fp, "\n");
    fprintf(fp, " oarconv ");
    fprintf(fp, "[-i OAR directory] [-o output directory] [-a adding assets directories that separated by ':'] [-f object xml file]\n");
    fprintf(fp, "         [-t terrain texture scale] [-c external convert command of jp2]\n");
    fprintf(fp, "         [-s start no.] [-e end no.]\n");
    fprintf(fp, "         [-x shift of x direction] [-y shift of y direction] [-z shift of z direction]\n");
    fprintf(fp, "         [-b] [-p] [-d] [-v] [-h]\n");
    fprintf(fp, "\n");

    fprintf(fp, "   -i : specify OAR directory. default is %s\n", OART_DEFAULT_INP_DIR);
    fprintf(fp, "   -o : specify output directory. default is %s\n", OART_DEFAULT_DAE_DIR);
    fprintf(fp, "   -f : specify object xml file. only specified file is converted.\n");
    fprintf(fp, "   -a : specify adding assets directories that separated by ':'. default is %s\n", OART_DEFAULT_AST_DIR);
    fprintf(fp, "   -t : specify terrain texture scale. default is %f\n", TRNT_DEFAULT_TEX_SCALE);
    fprintf(fp, "   -c : specify external convert command from jp2 to other image. default is \"%s\"\n", OART_JP2_DECOMP_COM);
    fprintf(fp, "   -s : specify start number of xml file. default is 0.\n");
    fprintf(fp, "   -e : specify end number of xml file. default is -1 (minus number means infinity).\n");
    fprintf(fp, "   -x : specify shift of x direction of all objects. default is 0.0\n");
    fprintf(fp, "   -y : specify shift of y direction of all objects. default is 0.0\n");
    fprintf(fp, "   -z : specify shift of z direction of all objects. default is 0.0\n");
    fprintf(fp, "   -b : output STL file(s) using BREP.\n");
    fprintf(fp, "   -p : when linked objects include phantom even one, saved to Phantom directory.\n");
    fprintf(fp, "   -d : debug mode. display debug information. \n");
    fprintf(fp, "   -v : display version information. \n");
    fprintf(fp, "   -h : display this help messages. \n");
    fprintf(fp, "\n");
    fprintf(fp, " ex.) oarconv -i OAR -a /usr/local/opensim/bin/assets/TexturesAssetSet:./assets -d\n");
    fprintf(fp, "\n");

    fflush(fp);
    return;
}


