/**
* Import Postèàóù for OAR   by Fumi.Iseki
*
*          ver1.1.0  2024/02/26
**/

#pragma once

#define  MATERIAL_BASE64_SIZE   32
#define  MATERIAL_PARAMS_SIZE   24
#define  MATERIAL_PARAMS_KIND   (MATERIAL_PARAMS_SIZE - 1)

#define  MATERIAL_SHADER_OBJ    "/OARSelectMaterial/Material/OAR_OBJ"
#define  MATERIAL_SHADER_MASK   "/OARSelectMaterial/Material/OAR_Mask"
#define  MATERIAL_SHADER_ALPHA  "/OARSelectMaterial/Material/OAR_Alpha"

#define  OBJ_PHANTOM_PREFIX     "UNC_"
#define  OBJ_COLLIDER_PREFIX    "UCP_"


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

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FDelegateHandle OnAssetPostImportHandle;
	void OnAssetPostImport(UFactory* Factory, UObject* CreatedObject);

	UMaterialInterface* SelectMaterialInterface(TArray<float> params);

	FString GetTexturePath(FString mtl_path, FString mtl_name);
	TArray<float> GetTextureParams(FString mtl_name);
};
