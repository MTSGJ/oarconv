/**
* Import Postèàóù for OAR   by Fumi.Iseki
*
*          ver1.1.0  2024/02/26
**/

#pragma once

#define  MATERIAL_BASE64_SIZE       32
#define  MATERIAL_PARAMS_SIZE       24
#define  MATERIAL_PARAMS_KIND       (MATERIAL_PARAMS_SIZE - 1)

#define  MATERIAL_SHADER_TREE       "/OARSelectMaterial/Material/OAR_Tree"
#define  MATERIAL_SHADER_EARTH      "/OARSelectMaterial/Material/OAR_Earth"
#define  MATERIAL_SHADER_GENERAL    "/OARSelectMaterial/Material/OAR_General"
#define  MATERIAL_SHADER_TRANS      "/OARSelectMaterial/Material/OAR_Trans"
#define  MATERIAL_SHADER_CUTOFF     "/OARSelectMaterial/Material/OAR_Trans"

#define  OBJ_PHANTOM_PREFIX         "UNC_"
#define  OBJ_COLLIDER_PREFIX        "UCP_"


#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Subsystems/ImportSubsystem.h"
#include "StaticMeshEditorSubsystem.h"

#include "OAREditorSubsystem.generated.h"


/**
 * OARSELECTMATERIAL_API
 */
UCLASS()
class OARSELECTMATERIAL_API UOAREditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

private:
    float colorRed      = 0.0f;
    float colorGreen    = 0.0f;
    float colorBlue     = 0.0f;
    float transparent   = 1.0f;  //
    float cutoff        = 0.0f;
    float shininess     = 0.0f;
    float glow          = 0.0f;
    float bright        = 0.0f;
    float light         = 0.0f;
    int   alphaMode     = 0;     // 0: NONE, 1: BLENDING, 2: MASKING, 3: EMISSIVE
    bool  hasAlpha      = false;
//  float shiftU        = 0.0f;
//  float shiftV        = 0.0f;
//  float scaleU        = 1.0f;
//  float scaleV        = 1.0f;
//  float rotate        = 0.0f;
    char  kind          = 'O';   // O: Object, T: Tree, G: Grass, E: Earth

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FDelegateHandle OnAssetPostImportHandle;
	void OnAssetPostImport(UFactory* Factory, UObject* CreatedObject);

	UMaterialInterface* SelectMaterialInterface();

	FString GetTexturePath(FString mtl_path, FString mtl_name);
	void    GetTextureParams(FString mtl_name);
};
