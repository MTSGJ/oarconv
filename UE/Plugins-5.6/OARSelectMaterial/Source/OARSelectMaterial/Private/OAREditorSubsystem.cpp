/**
* Import Post処理 for OAR   by Fumi.Iseki
*                                  v1.2.0 
*/

#include "OAREditorSubsystem.h"
#include "Subsystems/ImportSubsystem.h"


void UOAREditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//
	if (GEditor != NULL) {
		UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>();
		if (ImportSubsystem != NULL) {
			OnAssetPostImportHandle = ImportSubsystem->OnAssetPostImport.AddUObject(this, &UOAREditorSubsystem::OnAssetPostImport);
		}
	}
}


void UOAREditorSubsystem::Deinitialize()
{
	if (GEditor != NULL) {
		UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>();
		if (ImportSubsystem != NULL) {
			ImportSubsystem->OnAssetPostImport.Remove(OnAssetPostImportHandle);
		}
	}
	Super::Deinitialize();
}


void UOAREditorSubsystem::OnAssetPostImport(UFactory* Factory, UObject* CreatedObject)
{
	if (CreatedObject != NULL) {
		FString class_name = CreatedObject->GetClass()->GetName();

		// Static Mesh
		if (class_name.Equals(FString(TEXT("StaticMesh")))) {
			UStaticMesh* mesh = Cast<UStaticMesh>(CreatedObject);
			FString mesh_name = mesh->GetName();
			//UE_LOG(LogTemp, Log, TEXT("UOAREditorSubsystem: Processing for %s"), *mesh_name);

			// ファントム処理
			if (mesh_name.Find(FString(TEXT(OBJ_PHANTOM_PREFIX))) == 0) {
				UStaticMeshEditorSubsystem* MeshSubsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
				MeshSubsystem->RemoveCollisions(mesh);
			}

			int lod = 0;
			auto* mtlif = mesh->GetMaterial(lod);
			while (mtlif != NULL) {
				FString mtl_name = mtlif->GetName();
				//
				// パラメータの復元
				GetTextureParams(mtl_name);
				if (kind == '\0') {
					mtlif = mesh->GetMaterial(++lod);
					continue;
				}
				// マテリアルの選択
				UMaterialInterface* new_mtlif = SelectMaterialInterface();
				if (new_mtlif == NULL) {
					mtlif = mesh->GetMaterial(++lod);
					continue;
				}
				mesh->SetMaterial(lod, new_mtlif);		// Static Materialを登録

				// パラメータの適用
				UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(new_mtlif, mesh);
				if (material != NULL) {
					// Texture
					FString mtl_path = mtlif->GetPathName();
					FString texture_path = GetTexturePath(mtl_path, mtl_name);
					UTexture* texture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *texture_path));
					if (texture != NULL) {
						material->SetTextureParameterValue(FName(TEXT("Texture")), texture);
					}
					// Some Parameters
					material->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(colorRed, colorGreen, colorBlue));
					material->SetScalarParameterValue(FName(TEXT("Transparent")), transparent);
					material->SetScalarParameterValue(FName(TEXT("Cutoff")), cutoff);
					material->SetScalarParameterValue(FName(TEXT("Shininess")), shininess);
					material->SetScalarParameterValue(FName(TEXT("Glow")), glow);
					material->SetScalarParameterValue(FName(TEXT("Bright")), bright);
					material->SetScalarParameterValue(FName(TEXT("Light")),light);
					// Setup
					mesh->SetMaterial(lod, material);
				}
				mtlif = mesh->GetMaterial(++lod);
			}
		}

		// Material or MaterialInstanceConstant
		else if (CreatedObject->GetName().Find(FString(TEXT("MATERIAL_"))) == 0) {
			if (class_name.Equals(FString(TEXT("MaterialInstanceConstant")))) {
				//UE_LOG(LogTemp, Log, TEXT("Material Instance = %s"), *(CreatedObject->GetName()));
			}
			else if (class_name.Equals(FString(TEXT("Material")))) {
				//UE_LOG(LogTemp, Log, TEXT("Material = %s"), *(CreatedObject->GetName()));
			}
		}

		// Texture2D
		else if (class_name.Equals(FString(TEXT("Texture2D")))) {
			//UE_LOG(LogTemp, Log, TEXT("Texture2D = %s"), *(CreatedObject->GetName()));
		}
	}
}


FString UOAREditorSubsystem::GetTexturePath(FString mtl_path, FString mtl_name)
{
	FString _txpath = mtl_path.Left(mtl_path.Find(mtl_name));
	FString _txname = FString(TEXT("00000000-0000-0000-0000-000000000000"));
	//
	int32 _stt_name = mtl_name.Find(TEXT("_"));
	int32 _end_name = mtl_name.Find(TEXT("_"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, mtl_name.Len());

	int32 _len_txname = _end_name - _stt_name - 1;
	if (_len_txname > 0) {
		_txname = mtl_name.Left(_end_name).Right(_len_txname);
	}
	FString texture_name = _txpath + FString(TEXT("TEX_")) + _txname;

	return texture_name;
}


void  UOAREditorSubsystem::GetTextureParams(FString mtl_name)
{
	int32 _name_end = mtl_name.Find(TEXT("_"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, mtl_name.Len());
	FString _params_str = mtl_name.Right(mtl_name.Len() - _name_end - 1);

	if (_params_str.Len() != MATERIAL_BASE64_SIZE) return;
	_params_str = _params_str.Replace(TEXT("-"), TEXT("/"));
	//UE_LOG(LogTemp, Log, TEXT("params = %s"), *_params_str);

	TArray<uint8> dec;
	FBase64::Decode(_params_str, dec);
	if (dec.Num() != MATERIAL_PARAMS_SIZE) return;

    colorRed    = 1.0f - (float)dec[0] / 255.0f;
    colorGreen  = 1.0f - (float)dec[1] / 255.0f;
    colorBlue   = 1.0f - (float)dec[2] / 255.0f;
    transparent = 1.0f - (float)dec[3] / 255.0f;
    cutoff      = (float)dec[4] / 255.0f;
    shininess   = (float)dec[5] / 255.0f;
    glow        = (float)dec[6] / 255.0f;
    bright      = (float)dec[7] / 255.0f;
    light       = (float)dec[8] / 255.0f;
    alphaMode   = (int)dec[9]/10;           // 0: NONE, 1: BLENDING, 2: MASKING, 3: EMISSIVE
    int has_alp = (int)dec[9]%10;           // 0: No, 1: YES
    /*
    short[] tmp = new short[1];
    Buffer.BlockCopy(dec, 13, tmp, 0, 2);
    shiftU      = (float)tmp[0] / 2000.0f;
    Buffer.BlockCopy(dec, 15, tmp, 0, 2);
    shiftV      = (float)tmp[0] / 2000.0f;
    Buffer.BlockCopy(dec, 17, tmp, 0, 2);
    scaleU      = (float)tmp[0] / 100.0f;
    Buffer.BlockCopy(dec, 19, tmp, 0, 2);
    scaleV      = (float)tmp[0] / 100.0f;
    Buffer.BlockCopy(dec, 21, tmp, 0, 2);
    rotate      = (float)tmp[0] / 2000.0f;
    */
    kind = (char)dec[MATERIAL_PARAMS_KIND];

	if (cutoff > 0.9f) cutoff = 0.9f;
    if (has_alp == 1) hasAlpha = true;
    else              hasAlpha = false;
	return;
}


/**
  Aito Select for Material
*/
UMaterialInterface* UOAREditorSubsystem::SelectMaterialInterface()
{
	UMaterialInterface* mtlif = NULL;

	if (kind == 'T' || kind == 'G') {
		mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
			*FString(TEXT(MATERIAL_SHADER_TREE))));
	}
	else if (kind == 'E') {
		mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
			*FString(TEXT(MATERIAL_SHADER_EARTH))));
	}
	//
	else if (alphaMode == 0) {					// Alpha NONE
		//mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
		//	*FString(TEXT(MATERIAL_SHADER_GENERAL))));
    }
	else if (alphaMode == 1) {					// Alpha BLENDING
		if (hasAlpha) {
			mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
				*FString(TEXT(MATERIAL_SHADER_TRANS))));
		}
		else if (transparent < 0.9f) {
			if (transparent < 0.01f) {
				mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
					*FString(TEXT(MATERIAL_SHADER_CUTOFF))));
			}
			else {
				mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
					*FString(TEXT(MATERIAL_SHADER_TRANS))));
			}
		}
	}
	//
	else if (alphaMode == 2 && hasAlpha) {		// Alpha MASKING (Cutoff)
		mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
			*FString(TEXT(MATERIAL_SHADER_CUTOFF))));
	}
	//
	else if (alphaMode == 3 && hasAlpha) {		// Alpha EMISSIVE (not supported)
		mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
			*FString(TEXT(MATERIAL_SHADER_CUTOFF))));
	}

	if (mtlif == NULL) {
		mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
			*FString(TEXT(MATERIAL_SHADER_GENERAL))));
	}

	return mtlif;
}