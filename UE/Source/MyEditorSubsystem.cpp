/**
* Import Post処理 for OAR   by Fumi.Iseki
**/

#include "MyEditorSubsystem.h"
#include "Subsystems/ImportSubsystem.h"


void UMyEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//
	if (GEditor != NULL) {
		UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>();
		if (ImportSubsystem != NULL) {
			OnAssetPostImportHandle = ImportSubsystem->OnAssetPostImport.AddUObject(this, &UMyEditorSubsystem::OnAssetPostImport);
		}
	}
}


void UMyEditorSubsystem::Deinitialize()
{
	if (GEditor != NULL) {
		UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>();
		if (ImportSubsystem != NULL) {
			ImportSubsystem->OnAssetPostImport.Remove(OnAssetPostImportHandle);
		}
	}
	Super::Deinitialize();
}


void UMyEditorSubsystem::OnAssetPostImport(UFactory* Factory, UObject* CreatedObject)
{
	if (CreatedObject != NULL) {
		FString _class_name = CreatedObject->GetClass()->GetName();

		// Static Mesh
		if (_class_name.Equals(FString(TEXT("StaticMesh")))) {
			UStaticMesh* mesh = Cast<UStaticMesh>(CreatedObject);
			FString _mesh_name = mesh->GetName();

			// ファントム処理
			if (_mesh_name.Find(FString(TEXT(OBJ_PHANTOM_PREFIX))) == 0) {
				UStaticMeshEditorSubsystem* MeshSubsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
				MeshSubsystem->RemoveCollisions(mesh);
			}

			int i = 0;
			auto* mtlif = mesh->GetMaterial(i);
			while (mtlif != NULL) {
				FString _mtl_name = mtlif->GetName();
				// パラメータの復元
				TArray<float> params = GetTextureParams(_mtl_name);
				if (params[MATERIAL_PARAMS_SIZE - 1] == 0.0f) {
					mtlif = mesh->GetMaterial(++i);
					continue;
				}
				// マテリアルの選択
				UMaterialInterface* new_mtlif = SelectMaterialInterface(params);
				if (new_mtlif == NULL) {
					mtlif = mesh->GetMaterial(++i);
					continue;
				}
				// パラメータの適用
				UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(new_mtlif, NULL);
				if (material != NULL) {
					// Texture
					FString _obj_path = mtlif->GetPathName();
					FString texture_path = GetTexturePath(_obj_path, _mtl_name);
					UTexture* texture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *texture_path));
					if (texture != NULL) {
						material->SetTextureParameterValue(FName(TEXT("Texture")), texture);
					}
					// Some Parameters
					material->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(params[0], params[1], params[2]));
					material->SetScalarParameterValue(FName(TEXT("Transparent")), params[3]);
					material->SetScalarParameterValue(FName(TEXT("Cutoff")), params[4]);
					material->SetScalarParameterValue(FName(TEXT("Shininess")), params[5]);
					material->SetScalarParameterValue(FName(TEXT("Glow")), params[6]);
					material->SetScalarParameterValue(FName(TEXT("Bright")), params[7]);
					material->SetScalarParameterValue(FName(TEXT("Light")), params[8]);
					// Setup
					mesh->SetMaterial(i, material);
				}
				mtlif = mesh->GetMaterial(++i);
			}
		}

		// Material
		else if (CreatedObject->GetName().Find(FString(TEXT("MATERIAL_"))) == 0) {
			if (_class_name.Equals(FString(TEXT("MaterialInstanceConstant")))) {
				//UE_LOG(LogTemp, Log, TEXT("material = %s"), *(CreatedObject->GetName()));
			}
			else if (_class_name.Equals(FString(TEXT("Material")))) {
				//UE_LOG(LogTemp, Log, TEXT("material = %s"), *(CreatedObject->GetName()));
			}
		}
	}
}


UMaterialInterface* UMyEditorSubsystem::SelectMaterialInterface(TArray<float> params)
{
	UMaterialInterface* mtlif = NULL;

	int kind = (int)(params[23] + 0.5f);
	if (kind == 84 || kind == 71) {		// 'T' or 'G'
		mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
			*FString(TEXT(MATERIAL_SHADER_ALPHA))));
	}
	else if (kind == 79) {				// 'O'
		if (params[3] < 0.99f && params[4] < 0.01f) {	 // Transparent & Cutoff
			mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
				*FString(TEXT(MATERIAL_SHADER_ALPHA))));
		}
		else {
			mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
				*FString(TEXT(MATERIAL_SHADER_OBJ))));
		}
	}
	else if (kind == 69) {				// 'E'
		mtlif = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), NULL,
			*FString(TEXT(MATERIAL_SHADER_OBJ))));
	}

	return mtlif;
}


FString UMyEditorSubsystem::GetTexturePath(FString mtl_path, FString mtl_name)
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


TArray<float> UMyEditorSubsystem::GetTextureParams(FString mtl_name)
{
	TArray<float> params;
	params.Init(0.0, MATERIAL_PARAMS_SIZE);
	params[3] = 1.0f;	// Transparent

	int32 _name_end = mtl_name.Find(TEXT("_"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, mtl_name.Len());
	FString _params_str = mtl_name.Right(mtl_name.Len() - _name_end - 1);

	if (_params_str.Len() != MATERIAL_BASE64_SIZE) return params;
	_params_str = _params_str.Replace(TEXT("-"), TEXT("/"));
	//UE_LOG(LogTemp, Log, TEXT("params = %s"), *_params_str);

	TArray<uint8> dec;
	FBase64::Decode(_params_str, dec);
	if (dec.Num() != MATERIAL_PARAMS_SIZE) return params;

	params[0]  = 1.0f - (float)dec[0] / 255.0f;			// Red
	params[1]  = 1.0f - (float)dec[1] / 255.0f;			// Green
	params[2]  = 1.0f - (float)dec[2] / 255.0f;			// Blue
	params[3]  = 1.0f - (float)dec[3] / 255.0f;			// Alpha
	params[4]  = (float)dec[4] / 255.0f;				// Cutoff
	params[5]  = (float)dec[5] / 255.0f;				// Shinuness
	params[6]  = (float)dec[6] / 255.0f;				// Glow
	params[7]  = (float)dec[7] / 255.0f;				// Bright
	params[8]  = (float)dec[8] / 255.0f;				// Light
	params[23] = (float)dec[MATERIAL_PARAMS_SIZE - 1];	// Kind

	return params;
}
