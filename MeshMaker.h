#ifndef  __JBXL_OS_MESH_MAKER_H_
#define  __JBXL_OS_MESH_MAKER_H_


/**
@brief OpenSim用メッシュデータの生成 
@file  MeshMaker.h

@author  Fumi.Iseki 
@date    2014 9/16

@sa OpenSim/Region/Physics/Meshing/Meshmerizer.cs
*/


#include <vector>
#include <algorithm>

#include "Vector.h"
#include "Rotation.h"
#include "ColladaTool.h"

#include "PrimMesher.h"
#include "SculptMesher.h"
#include "TerrainMesher.h"
#include "MaterialTool.h"
#include "LogDocTool.h"

#include "Jpeg2KTool.h"
#include "TgaTool.h"

#include "gz_tool.h"
#include "llsd_tool.h"




namespace  jbxl {


///////////////////////////////////////////////////////////////////////////////////////////////
//

MeshObjectData* MeshObjectDataFromPrimShape(PrimBaseShape shape, tList* resourceList, bool useBrep, bool addParam);

PrimMesh        GeneratePrimMesh(PrimMeshParam param);
TriPolyData*    TriPolyDataFromPrimMesh(PrimMesh primMesh, int* fnum, int* pnum);


///////////////////////////////////////////////////////////////////////////////////////////////
// for Scuplpted Prim
// @sa http://wiki.secondlife.com/wiki/Sculpted_Prims:_Technical_Explanation

FacetBaseData*  FacetBaseDataFromSculptImage(MSGraph<uByte> grd,  int type);
FacetBaseData*  FacetBaseDataFromSculptJP2K(const char* jpegfile, int type);

TriPolyData*    TriPolyDataFromSculptImage(MSGraph<uByte> grd,  int type, int* pnum);
TriPolyData*    TriPolyDataFromSculptJP2K(const char* jpegfile, int type, int* pnum);

TriPolyData*    GenerateGrayScaleSculpt(int* pnum);


///////////////////////////////////////////////////////////////////////////////////////////////
// for LLMesh
// @sa http://wiki.secondlife.com/wiki/Mesh/Mesh_Asset_Format

TriPolyData*    TriPolyDataFromLLMesh(uByte* mesh, int sz, int* fnum, int* pnum);
TriPolyData*    TriPolyDataFromLLMeshFile(const char* filename,  int* fnum, int* pnum);

tXML*           GetLLsdXMLFromLLMesh(uByte* mesh, int sz, const char* key);

Vector<float>*  GetLLMeshPositionDomainMaxMin(tXML* xml, int facet, bool max=true);
UVMap<float>*   GetLLMeshTexCoordDomainMaxMin(tXML* xml, int facet, bool max=true);

#define  GetLLMeshPositionDomainMax(x, v)  GetLLMeshPositionDomainMaxMin((x), (v), true)
#define  GetLLMeshPositionDomainMin(x, v)  GetLLMeshPositionDomainMaxMin((x), (v), false)
#define  GetLLMeshTexCoordDomainMax(x, v)  GetLLMeshTexCoordDomainMaxMin((x), (v), true)
#define  GetLLMeshTexCoordDomainMin(x, v)  GetLLMeshTexCoordDomainMaxMin((x), (v), false)

float  LLMeshUint16toFloat(uByte* ptr, float max, float min);


///////////////////////////////////////////////////////////////////////////////////////////////
// for Terrain Mesh

FacetBaseData*  FacetBaseDataFromTerrainImage(MSGraph<float> grd, Vector<float> shift, bool left, bool right, bool top, bool bottom, bool autosea);
FacetBaseData*  FacetBaseDataFromTerrainR32(char* r32file, int xsize, int ysize, Vector<float> shift, bool autosea);            // 未使用

TriPolyData*    TriPolyDataFromTerrainImage(MSGraph<float> grd, int* pnum, Vector<float> shift, bool left, bool right, bool top, bool bottom, bool autosea);// 未使用
TriPolyData*    TriPolyDataFromTerrainR32(char* r32file, int* pnum, int xsize, int ysize, Vector<float> shift, bool autosea);   // 未使用



}       // namespace

#endif

