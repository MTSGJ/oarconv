
/**
@brief OpenSimの Tree/Grassのサポート
@file  TreeTool.cpp

@author  Fumi.Iseki & D.Mizumaki
@date    2015/06/14
*/

#include "TreeTool.h"


using namespace jbxl;


//
void  TreeTool::init(void)
{
    shape.PCode  = PRIM_PCODE_TREE;
    shape.State  = TREM_TREE_PINE1;
    addname      = init_Buffer();

    alphaTexture.init();
    for (int i=0; i<TREM_TREE_NUM;  i++) treeParam[i].init();
    for (int i=0; i<TREM_GRASS_NUM; i++) grassParam[i].init();

    alphaTexture.setName("00000000-0000-2222-3333-100000001007");           // Alpha Chanel Texture

    // Tree
    treeParam[ 0].texture.setName("ccc1cae5-1bef-4a36-a2fa-ef07e2d4cd54");  // Pine 1
    treeParam[ 1].texture.setName("222d620a-3b4c-4736-9c78-d112f23ad97b");  // Oak
    treeParam[ 2].texture.setName("7e1370c0-3057-462a-95c7-c70ff973903f");  // Tropical Bush 1
    treeParam[ 3].texture.setName("6b45034e-ecba-4560-b950-94123d579a02");  // Palm 1
    treeParam[ 4].texture.setName("814835c2-3be4-4583-a64d-c07e03805524");  // Dogwood
    treeParam[ 5].texture.setName("a51bf1c7-6728-40d7-801c-e6b1997a2e98");  // Tropical Bush 2
    treeParam[ 6].texture.setName("aa0065d1-a368-4bab-a4ae-541a78254f4c");  // Palm 2
    treeParam[ 7].texture.setName("7cf86bc2-4dc8-4df8-afdb-b8a9189a28b1");  // Cypress 1
    treeParam[ 8].texture.setName("1d260eb6-f593-4414-b549-dcef12897d29");  // Cypress 2
    treeParam[ 9].texture.setName("0452407d-dcc5-4377-9fba-49cfb33112df");  // Pine 2
    treeParam[10].texture.setName("0f0eafeb-a92b-47d6-b797-138950407dde");  // Plumeria
    treeParam[11].texture.setName("413b0f15-485b-4c48-82c3-3a358b92df23");  // Winter Pine 1
    treeParam[12].texture.setName("13d942aa-221d-4e78-a40f-1294f340568b");  // Winter Aspen
    treeParam[13].texture.setName("0509ebda-ccc6-448e-812d-9529452b123f");  // Winter Pine 2
    treeParam[14].texture.setName("a2714a3e-597d-4c1f-a983-4e76700402cc");  // Eucalyptus
    treeParam[15].texture.setName("b84582d5-8607-401e-a02e-6dd6cae83b19");  // Fern
    treeParam[16].texture.setName("63f8aa88-085d-4012-bad2-b0dcdd80d9e6");  // Eelgrass
    treeParam[17].texture.setName("9d18e7e6-08e2-493a-8da9-ff8889433c06");  // Sea Sword
    treeParam[18].texture.setName("ec0cae75-47b4-45d8-b10b-c63c4e056901");  // Kelp 1
    treeParam[19].texture.setName("a2b37c1d-1abe-46d9-854f-4bedf79e98f3");  // Beach Grass 1
    treeParam[20].texture.setName("bd06014d-7597-4441-9be1-4881689bb7dd");  // Kelp 2
    treeParam[21].texture.init();                                           // for TEST Tree

    treeParam[ 0].size = Vector<float>(3.2f, 0.0001f, 6.15f);               // Pine 1           1024x1968   6.15
    treeParam[ 1].size = Vector<float>(1.0f, 0.0001f, 1.54f);               // Oak              1024x1572   1.54
    treeParam[ 2].size = Vector<float>(0.6f, 0.0001f, 0.625f);              // Tropical Bush 1  1024x1066   0.625

    treeParam[ 3].size = Vector<float>(1.2f, 0.0001f, 2.26f);               // Palm 1           1024x1928   2.26
    treeParam[ 4].size = Vector<float>(0.8f, 0.0001f, 1.52f);               // Dogwood          1024x1948   1.52
    treeParam[ 5].size = Vector<float>(0.5f, 0.0001f, 0.555f);              // Tropical Bush 2  1024x1136   0.555

    treeParam[ 6].size = Vector<float>(1.2f, 0.0001f, 2.26f);               // Palm 2           1024x1928   2.26
    treeParam[ 7].size = Vector<float>(2.0f, 0.0001f, 3.79f);               // Cypress 1        1024x1940   3.79
    treeParam[ 8].size = Vector<float>(2.3f, 0.0001f, 4.40f);               // Cypress 2        1024x1960   4.40

    treeParam[ 9].size = Vector<float>(2.7f, 0.0001f, 5.20f);               // Pine 2           1024x1972   5.20
    treeParam[10].size = Vector<float>(1.3f, 0.0001f, 1.52f);               // Plumeria         1024x1195   1.52
    treeParam[11].size = Vector<float>(1.0f, 0.0001f, 1.92f);               // Winter Pine 1    1024x1968   1.92

    treeParam[12].size = Vector<float>(1.0f, 0.0001f, 1.89f);               // Winter Aspen     1024x1932   1.89
    treeParam[13].size = Vector<float>(0.65f,0.0001f, 1.23f);               // Winter Pine 2    1024x1936   1.23
    treeParam[14].size = Vector<float>(2.5f, 0.0001f, 3.79f);               // Eucalyptus       1024x1552   3.79

    treeParam[15].size = Vector<float>(0.25f,0.0001f, 0.250f);              // Fern             1024x1024   0.250
    treeParam[16].size = Vector<float>(0.25f,0.0001f, 0.469f);              // Eelgrass         1024x1920   0.469
    treeParam[17].size = Vector<float>(0.15f,0.0001f, 0.168f);              // Sea Sword        1024x1148   0.168

    treeParam[18].size = Vector<float>(0.42f,0.0001f, 0.758f);              // Kelp 1           1024x1848   0.758
    treeParam[19].size = Vector<float>(0.3f, 0.0001f, 0.369f);              // Beach Grass 1    1024x1260   0.369
    treeParam[20].size = Vector<float>(0.62f,0.0001f, 1.16f);               // Kelp 2           1024x1920   1.16

    treeParam[21].size = Vector<float>(1.0f, 0.0001f, 1.0f);                // for TEST Tree

    treeParam[ 7].num_panel = 3;    // Cypress 1
    treeParam[ 8].num_panel = 3;    // Cypress 2
    treeParam[12].num_panel = 6;    // Winter Aspen
    treeParam[13].num_panel = 6;    // Winter Pine 2
    treeParam[15].num_panel = 6;    // Fern
    treeParam[16].num_panel = 6;    // Eelgrass
    treeParam[17].num_panel = 6;    // Sea Sword
    treeParam[18].num_panel = 2;    // Kelp 1
    treeParam[20].num_panel = 2;    // Kelp 2

    // Grass
    grassParam[0].texture.setName("6c4727b8-ac79-ba44-3b81-f9aa887b47eb");  // Medium
    grassParam[1].texture.setName("79504bf5-c3ec-0763-6563-d843de66d0a1");  // Short 2
    grassParam[2].texture.setName("6c4727b8-ac79-ba44-3b81-f9aa887b47eb");  // Tall Green
    grassParam[3].texture.setName("99bd60a2-3250-efc9-2e39-2fbcadefbecc");  // Short 1
    grassParam[4].texture.setName("7a2b3a4a-53c2-53ac-5716-aac7d743c020");  // Tall Brown
    grassParam[5].texture.setName("8f458549-173b-23ff-d4ff-bfaa5ea2371b");  // Leafy Ground Cover
    grassParam[6].texture.init();                                           // for TEST Grass

    grassParam[0].size = Vector<float>(0.7f, 0.0001f, 0.7f);                // Medium
    grassParam[1].size = Vector<float>(0.5f, 0.0001f, 0.5f);                // Short 2
    grassParam[2].size = Vector<float>(1.0f, 0.0001f, 1.0f);                // Tall Green
    grassParam[3].size = Vector<float>(0.5f, 0.0001f, 0.5f);                // Short 1
    grassParam[4].size = Vector<float>(1.0f, 0.0001f, 1.0f);                // Tall Brown
    grassParam[5].size = Vector<float>(1.0f, 0.0001f, 1.0f);                // Leafy Ground Cover
    grassParam[6].size = Vector<float>(1.0f, 0.0001f, 1.0f);                // for TEST Grass

    return;
}


//
void  TreeTool::set_shape(PrimBaseShape pbs)
{
    shape.free();
    shape.dup(pbs);

    if (shape.PCode==PRIM_PCODE_NEWTREE || shape.PCode==PRIM_PCODE_TREE) {
        if (shape.State<0 || shape.State>=TREM_TREE_NUM)  shape.State = 0;
    }
    else if (shape.PCode==PRIM_PCODE_GRASS) {
        if (shape.State<0 || shape.State>=TREM_GRASS_NUM) shape.State = 0;
    }
}


//
void  TreeTool::free(void)
{
    free_Buffer(&addname);

    alphaTexture.free();
    for (int i=0; i<TREM_TREE_NUM;  i++) treeParam[i].free();
    for (int i=0; i<TREM_GRASS_NUM; i++) grassParam[i].free();
    //
    shape.free();
}


//
char*  TreeTool::GetTreeTexture(int kind)  
{
    if (shape.PCode==PRIM_PCODE_NEWTREE || shape.PCode==PRIM_PCODE_TREE) {
        if (kind==-1) kind = shape.State;
        if (kind>=0 && kind<TREM_TREE_NUM) return treeParam[kind].texture.getName();
    }
    return NULL; 
}


//
char*  TreeTool::GetGrassTexture(int kind)  
{
    if (shape.PCode==PRIM_PCODE_GRASS) {
        if (kind==-1) kind = shape.State;
        if (kind>=0 && kind<TREM_GRASS_NUM) return grassParam[kind].texture.getName();
    }
    return NULL; 
}


/**
木データの生成．

@param pbs        基本情報が格納された形状変数．木の種類は pbs.Stateに格納されている．
@param ndiv       木を構成する平面の数．

@return  Meshデータ．
*/
MeshObjectData*  TreeTool::GenerateTree(PrimBaseShape pbs, int ndiv)
{
    if (pbs.State<0 || pbs.State>=TREM_TREE_NUM) pbs.State = 0;
    set_shape(pbs);
    DEBUG_MODE PRINT_MESG("TreeTool::GenerateTree: number of tree unit is %d\n", pbs.State);

    if (ndiv<=0) ndiv = treeParam[pbs.State].num_panel;

    PrimBaseShape bases;
    PrimMeshParam param;
    bases.GenerateBaseParam(PRIM_TYPE_BOX);
    param.GetParamFromBaseShape(bases);
    PrimMesh primMesh = GeneratePrimMesh(param);
    bases.free();
    param.free();

    int trino;      // 三角ポリゴンの数
    int facetno;    // 面の数
    TriPolygonData* tridata = TriPolygonDataFromPrimMesh(primMesh, &facetno, &trino);
    primMesh.free();

    for (int i=0; i<trino; i++) {
        tridata[i].execScale(treeParam[shape.State].size);
    }
    TriPolygonData* orgdata = dupTriPolygonData(tridata, trino);

    double th = PI/ndiv;
    for (int n=1; n<ndiv; n++) {
        TriPolygonData* dupdata = dupTriPolygonData(orgdata, trino);
        for (int i=0; i<trino; i++) {
            dupdata[i].execRotate(Quaternion<double>(th*n, Vector<double>(0.0, 0.0, 1.0)));
        }
        tridata = joinTriPolygonData(tridata, trino*n, dupdata, trino);
    }
    freeTriPolygonData(orgdata, trino);
    trino   *= ndiv;
    facetno *= ndiv;
    
    // Material Paramter
    MaterialParam mparam[3];
    mparam[0].setTextureName(alphaTexture.getName());
    mparam[1].setTextureName(treeParam[shape.State].texture.getName());
    mparam[2].setTextureName(treeParam[shape.State].texture.getName());
    mparam[2].texture.setFlipU(true);

    for (int i=0; i<3; i++) {
        //mparam[i].setTransparent(MTRL_DEFAULT_ALPHA);
        mparam[i].texture.setColor(1.0f, 1.0f, 1.0f, 1.0f);
        mparam[i].texture.setAlphaMode(MATERIAL_ALPHA_MASKING);
        mparam[i].texture.setAlphaCutoff(0.5);
        mparam[i].setKind('T');                         // T: Tree
        char* paramstr = mparam[i].getBase64Params();
        mparam[i].setParamString(paramstr);
        if (paramstr!=NULL) ::free(paramstr);
    }

    MeshObjectData* data = new MeshObjectData((char*)shape.ObjectName.buf);
    for (int i=0; i<facetno; i++) {
        data->addData(tridata, trino, i, NULL, false);
        //data->addData(tridata, trino, i, mparam, false);
    }
    freeTriPolygonData(tridata, trino);

    for (int i=0; i<ndiv; i++) { 
        // 6面
        data->setMaterialParam(mparam[0]);
        data->setMaterialParam(mparam[1]);
        data->setMaterialParam(mparam[0]);
        data->setMaterialParam(mparam[2]);
        data->setMaterialParam(mparam[0]);
        data->setMaterialParam(mparam[0]);
    }
    for (int i=0; i<3; i++) mparam[i].free();

    double scale = (shape.affineTrans.getScaleX() + shape.affineTrans.getScaleY() + shape.affineTrans.getScaleZ())/3.0;

/*
    AffineTrans<double> affine;
    affine.setScale(scale, scale, scale); 
    affine.rotate = shape.affineTrans.rotate;
    affine.setShift(shape.affineTrans.shift.x, shape.affineTrans.shift.y, shape.affineTrans.shift.z);
    data->setAffineTrans(affine);
    affine.free();*/

    data->setAffineTrans(shape.affineTrans);
    data->affineTrans->setScale(scale, scale, scale);
    data->affineTrans->computeMatrix();

    primMesh.clear();
    return data;
}


/**
草データの生成．

@param pbs        基本情報が格納された形状変数．木の種類は pbs.Stateに格納されている．
@param terrain    地形情報ツールへのポインタ．草の Z座標を決めるために必要．NULLなら Z座標は 0.0

@return  data: Meshデータへのポインタ．
*/
MeshObjectData*  TreeTool::GenerateGrass(PrimBaseShape pbs, TerrainTool* terrain)
{
    if (pbs.State<0 || pbs.State>=TREM_GRASS_NUM) pbs.State = 0;
    set_shape(pbs);
    int num_grass = (int)(shape.affineTrans.getScaleX() * shape.affineTrans.getScaleY() * TREM_GRASS_NUM_RATE);
    DEBUG_MODE PRINT_MESG("TreeTool::GenerateGrass: number of grass unit is %d\n", num_grass);

    PrimBaseShape bases;
    PrimMeshParam param;
    bases.GenerateBaseParam(PRIM_TYPE_BOX);
    param.GetParamFromBaseShape(bases);
    PrimMesh primMesh = GeneratePrimMesh(param);
    bases.free();
    param.free();

    int trino;      // 三角ポリゴンの数
    int facetno;    // 面の数
    TriPolygonData* orgdata = TriPolygonDataFromPrimMesh(primMesh, &facetno, &trino);
    primMesh.free();

    //
    for (int i=0; i<trino; i++) {
        orgdata[i].execScale(grassParam[shape.State].size);
    }
    TriPolygonData* tridata = NULL;
    
    int gnum = 0;
    for (int n=0; n<num_grass; n++) {
        float xx = (float)((Frand()-0.5f) * shape.affineTrans.getScaleX()); 
        float yy = (float)((Frand()-0.5f) * shape.affineTrans.getScaleY()); 
        float height = -(float)shape.affineTrans.getShiftZ();
        bool  valid_pos = true;
        //
        if (terrain!=NULL) {
            float aa = xx + (float)shape.affineTrans.getShiftX();   // + terrain->xsize/2.0f;
            float bb = yy + (float)shape.affineTrans.getShiftY();   // + terrain->ysize/2.0f;
            if (aa<0.0 || bb<0.0 || aa>(float)terrain->xsize-1.0f || bb>(float)terrain->ysize-1.0f) {   // ex. 0.0 - 255.0
                valid_pos = false;
            }
            else {
                int i0 = (int)aa;   // ex. 0 - 255
                int j0 = (int)bb;   // ex. 0 - 255
                int i1 = i0 + 1;
                int j1 = j0 + 1;
                if (i1>=terrain->xsize) i1 = terrain->xsize - 1;
                if (j1>=terrain->ysize) j1 = terrain->ysize - 1;
                float alph = aa - i0;
                float beta = bb - j0;
                float h0 = (float)terrain->height(i0, terrain->ysize-1-j0);
                float h1 = (float)terrain->height(i1, terrain->ysize-1-j0);
                float h2 = (float)terrain->height(i0, terrain->ysize-1-j1);
                float h3 = (float)terrain->height(i1, terrain->ysize-1-j1);
                //height += h0*(1.0f-alph)*(1.0f-beta) + h1*alph*(1.0f-beta) + h2*(1.0f-alph)*beta + h3*alph*beta - terrain->waterHeight;
                height += h0*(1.0f-alph)*(1.0f-beta) + h1*alph*(1.0f-beta) + h2*(1.0f-alph)*beta + h3*alph*beta;
            }
        }
        //
        if (valid_pos) {
            TriPolygonData* dupdata = dupTriPolygonData(orgdata, trino);
            double th = (float)(Frand()*PI2);
            //
            for (int i=0; i<trino; i++) {
                dupdata[i].execRotate(Quaternion<double>(th*n, Vector<double>(0.0, 0.0, 1.0)));
                dupdata[i].execShift(Vector<double>(xx, yy, height + grassParam[shape.State].size.z*3.0/8.0)); // *(1/4 + 1/8)
            }
            tridata = joinTriPolygonData(tridata, trino*gnum, dupdata, trino);
            gnum++;
        }
    }
    //
    freeTriPolygonData(orgdata, trino);
    trino   *= gnum;
    facetno *= gnum;

    MaterialParam mparam[3];
    mparam[0].setTextureName(alphaTexture.getName());
    mparam[1].setTextureName(grassParam[shape.State].texture.getName());
    mparam[2].setTextureName(grassParam[shape.State].texture.getName());
    mparam[2].texture.setFlipU(true);

    for (int i=0; i<3; i++) {
        //mparam[i].setTransparent(MTRL_DEFAULT_ALPHA);
        mparam[i].texture.setColor(1.0, 1.0, 1.0, 1.0);
        mparam[i].texture.setAlphaMode(MATERIAL_ALPHA_MASKING);
        mparam[i].texture.setAlphaCutoff(0.5);
        mparam[i].setKind('G');                         // G: Grass
        char* paramstr = mparam[i].getBase64Params();
        mparam[i].setParamString(paramstr);
        if (paramstr!=NULL) ::free(paramstr);
    }

    MeshObjectData* data = new MeshObjectData((char*)shape.ObjectName.buf);
    for (int i=0; i<facetno; i++) {
        data->addData(tridata, trino, i, NULL, false);
    }
    freeTriPolygonData(tridata, trino);

    for (int i=0; i<gnum; i++) { 
        data->setMaterialParam(mparam[0]);
        data->setMaterialParam(mparam[1]);
        data->setMaterialParam(mparam[0]);
        data->setMaterialParam(mparam[2]);
        data->setMaterialParam(mparam[0]);
        data->setMaterialParam(mparam[0]);
    }
    for (int i=0; i<3; i++) mparam[i].free();

    data->setAffineTrans(shape.affineTrans);
    data->affineTrans->setScale(1.0, 1.0, 1.0);
    data->affineTrans->computeMatrix();

    primMesh.clear();
    return data;
}



/////////////////////////////////////////////////////////////////////////////////

void TreeParam::init(void)
{
    //texture.init();
    size.set (1.0, 0.0001, 1.0);
    shift.set(0.0, 0.0);
    scale.set(1.0, 1.0);

    num_panel = 4;
}


//
void GrassParam::init(void)
{
    texture.init();
    size.set(1.0, 0.0001, 1.0);
}

