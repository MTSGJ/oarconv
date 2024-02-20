/**
* Import Postèàóù for OAR   by Fumi.Iseki
*
*          ver1.0É¿  2024/02/16
**/

#pragma once

#define  MATERIAL_BASE64_SIZE   32
#define  MATERIAL_PARAMS_SIZE   24
#define  MATERIAL_PARAMS_KIND   (MATERIAL_PARAMS_SIZE - 1)

#define  MATERIAL_SHADER_OBJ    "/Game/Material/OpenSim_OBJ"
#define  MATERIAL_SHADER_MASK   "/Game/Material/OpenSim_Mask"
#define  MATERIAL_SHADER_ALPHA  "/Game/Material/OpenSim_Alpha"

#define  OBJ_PHANTOM_PREFIX     "UNC_"
#define  OBJ_COLLIDER_PREFIX    "UCP_"


#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Subsystems/ImportSubsystem.h"
#include "StaticMeshEditorSubsystem.h"
//#include "Editor/StaticMeshEditor/Public/StaticMeshEditorSubsystem.h"

#include "MyEditorSubsystem.generated.h"


/**
 * MYPROJECT_API
 */
UCLASS()
class MYPROJECT_API UMyEditorSubsystem : public UEditorSubsystem
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
