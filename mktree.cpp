/** 
@brief   ツリープリムの生成のテスト

@author  Fumi.Iseki 
@date   2015 5/31
@version 1.0
*/


#include "oarconv.h"


using namespace jbxl;



int main(int argc, char** argv)
{
    Buffer inpdir  = init_Buffer();
    Buffer outdir  = init_Buffer();
    Buffer adddir  = make_Buffer_bystr("./");
    Buffer texture = init_Buffer();
    float  xsize   = 1.0;
    float  ysize   = 1.0;
    int    pnum    = 4;

    DebugMode = ON;

    if (argc>1) copy_s2Buffer(argv[1], &texture);
    for (int i=1; i<argc; i++) {
        if      (!strcmp(argv[i], "-i")) { if (i!=argc-1) copy_s2Buffer(argv[i+1], &inpdir);}
        else if (!strcmp(argv[i], "-o")) { if (i!=argc-1) copy_s2Buffer(argv[i+1], &outdir);}
        else if (!strcmp(argv[i], "-a")) { if (i!=argc-1) copy_s2Buffer(argv[i+1], &adddir);}
        else if (!strcmp(argv[i], "-x")) { if (i!=argc-1) xsize = (float)atof(argv[i+1]);}
        else if (!strcmp(argv[i], "-y")) { if (i!=argc-1) ysize = (float)atof(argv[i+1]);}
        else if (!strcmp(argv[i], "-n")) { if (i!=argc-1) pnum  = (int)atoi(argv[i+1]);}
    }

    if (texture.buf==NULL) {
        print_message("Usage.... %s texture_uuid [オプション]\n", argv[0]);
        print_message("ex.)      %s 0452407d-dcc5-4377-9fba-49cfb33112df  -i OAR\n", argv[0]);
        exit(1);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////
    // main process
    OARTool oar;
    oar.SetPathInfo((char*)inpdir.buf, (char*)outdir.buf, (char*)outdir.buf, (char*)adddir.buf);
    oar.GetDataInfo();
//  oar.MakeOutputFolder(makedir);
//  oar.SetShift(xshift, yshift, zshift);

    TreeTool*   tree = oar.GetTreeTool();
    ColladaXML* dae  = new ColladaXML();

    PrimBaseShape shape;
    shape.State = 21;
    shape.PCode = PRIM_PCODE_TREE;
    tree->treeParam[shape.State].texture.setName((char*)texture.buf);
    tree->treeParam[shape.State].size    = Vector<float>(xsize, 0.0001, ysize);

    MeshObjectData* data = tree->GenerateTree(shape, pnum, FALSE);
    MeshObjectNode* node = data->nodelist;
    while (node!=NULL) {
        if (node->material_param.enable) {
            // convert texture
            oar.ConvertTexture(node->material_param.getTextureName(), node->material_param.getAdditionalName(), MTRL_IMAGE_TYPE);
            node->material_param.setupFullName(MTRL_IMAGE_TYPE);
        }
        node = node->next;
    }
    dae->addObject(data, false);
    dae->outputFile("test_tree.dae", oar.get_outpath(true));

    /////////////////////////////////////////////////////////////
    dae->free();
    tree->free();
    oar.free();
    shape.free();
    //
    freeMeshObjectData(data);
    freeColladaXML(dae);
    
    free_Buffer(&inpdir);
    free_Buffer(&outdir);
    free_Buffer(&adddir);
    free_Buffer(&texture);

    return 0;
}

