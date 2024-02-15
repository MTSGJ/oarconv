// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEditorSubsystem.h"
#include "Subsystems/EditorAssetSubsystem.h"


void UMyEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (GEditor == NULL) return;
	UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>();

	if (ImportSubsystem != NULL)
	{
		OnAssetPostImportHandle = ImportSubsystem->OnAssetPostImport.AddUObject(this, &UMyEditorSubsystem::OnAssetPostImport);
		OnAssetPreImportHandle = ImportSubsystem->OnAssetPreImport.AddUObject(this, &UMyEditorSubsystem::OnAssetPreImport);
	}
}



void UMyEditorSubsystem::OnAssetPostImport(UFactory* Factory, UObject* CreatedObject)
{

	if (CreatedObject != NULL) {
		UClass* _uclass = CreatedObject->GetClass();
		if (_uclass == NULL) return;

		FString _class = _uclass->GetName();


		if (_class.Equals(FString(TEXT("StaticMesh")))) {
			UStaticMesh* mesh = Cast<UStaticMesh>(CreatedObject);

			UE_LOG(LogTemp, Log, TEXT("Mesh Name = %s"), *(CreatedObject->GetName()));
			
			int i = 0;
			auto* mtlif = mesh->GetMaterial(i);

			while (mtlif != NULL) {
				FString _path = mtlif->GetPathName();
				FString _name = mtlif->GetName();
				//
				FString texture_path = GetTexturePath(_path, _name);
				TArray<float> params = GetTextureParams(_name);

				UE_LOG(LogTemp, Log, TEXT("Material Name = %d: %s (%f, %f, %f) %f"), i, *_name, 
					params[0], params[1], params[2], params[23]);

				UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(mtlif, NULL);
				if (material == NULL) {
					mtlif = mesh->GetMaterial(++i);
					continue;
				}

				UTexture* texture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, *texture_path));
				if (texture != NULL) {
					material->SetTextureParameterValue(FName(TEXT("Texture")), texture);
				}

				material->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(params[0], params[1], params[2]));
				material->SetVectorParameterValue(FName(TEXT("BlendMode")), FLinearColor(params[0], params[1], params[2]));
				material->SetScalarParameterValue(FName(TEXT("Shininess")), params[5]);
				//material->Get


				mesh->SetMaterial(i, material);
				mtlif = mesh->GetMaterial(++i);





			//CreatedObject->GetAssetRegistryTags(asset);

			//for (auto& aaa : masset) {
				//aaa.ToSt

			
			//TArray
			//TArray<FStaticMaterial> smat = mesh->GetStaticMaterials();
			//int i = 0;
			//for (auto& mat : smat) {
			//	//auto* matif = mesh->GetMaterial(i);
			//	//FString mname = matif->GetName();
			//
			//	auto matif = mat.MaterialInterface;
			//	FString mname = matif.GetName();
			//	UE_LOG(LogTemp, Log, TEXT("material -=========> %d: %s"),i, *mname);
			//
			//	UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(matif, NULL);
			//	material->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(0.0f, 0.0f, 1.0f));
			//	mesh->SetMaterial(i, material);
			//	i++;
			//	//delete &mat;
			//}
			//mesh->SetStaticMaterials(smat);
			

			


				/*
				TArray<FMaterialParameterInfo> pi;
				TArray<FGuid> ids;
				mtlif->GetAllTextureParameterInfo(pi, ids);
				for (auto aaa : pi) {	
					UE_LOG(LogTemp, Log, TEXT("Texture Param -=========> %s %s %d"), *(aaa.Name.ToString()), *(aaa.ToString()), aaa.Index);
				}*/

				/*
				UTexture* text;
				if (mtlif->GetTextureParameterValue(FName(TEXT("Texture")), text)) {
					//mtlif->GetTextureParameterValue(FName(TEXT("map_Kd")), text);

					// UTexture * texture = LoadObject<UTexture>（NULL, TEXT（"Floors.Stone"）, NULL, 0, NULL）;
					UE_LOG(LogTemp, Log, TEXT("Texture Param -=========> %s"), *(text->GetName()));
					UE_LOG(LogTemp, Log, TEXT("Texture FFF Param -=========> %s"), *(text->GetFullName()));
				}

				TArray<UTexture*> ptex;
				mtlif->GetUsedTextures(ptex, EMaterialQualityLevel::Num, false, ERHIFeatureLevel::Num, true);
				for (int32 TextureIndex = 0; TextureIndex < ptex.Num(); ++TextureIndex) {
					UTexture2D* Texture = Cast<UTexture2D>(ptex[TextureIndex]);
					UE_LOG(LogTemp, Log, TEXT("Texture2D Param -=========> %s %d %s"), *(Texture->GetName()), Texture->GetUniqueID()
						, *(Texture->GetExternalTextureGuid().ToString()));
				}*/

				//UTexture* texture = LoadObject<UTexture>(NULL, TEXT("6b2665ac-f74f-4737-873e-e1861843c3ab.tga"), NULL, 0, NULL);
				//UTexture* texture = NewObject<UTexture>(NULL, TEXT("6b2665ac-f74f-4737-873e-e1861843c3ab"), RF_Public | RF_Standalone);				
			}

			
			
			//UMaterialInterface* matif = mesh->GetMaterial(0);
			//auto* matif = mesh->GetMaterial(0);
			//FString mname = matif->GetName();
			//UE_LOG(LogTemp, Log, TEXT("material -=========> %s"), *mname);

			//UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(matif, NULL);

			//material->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(1.0f, 0.0f, 0.0f));
			//material->SetVectorParameterValue(FName(TEXT("DiffuseColor")), FLinearColor(1.0f, 0.0f, 0.0f));
			//material->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(1.0f, 0.0f, 0.0f));

			//mesh->SetMaterial(0, material);
		}




/*
		if (Factory != NULL) {
			FName _name = Factory->GetFName();
			UE_LOG(LogTemp, Log, TEXT("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD> %s"), *_name.ToString());
		}
		*/
		

		/*
		//UE_LOG(LogTemp, Log, TEXT("SSS> %s"), *_name);
		if (_namexx.Find(FString(TEXT("MATERIAL_"))) == 0) {
			UE_LOG(LogTemp, Log, TEXT("GGGGGGGGGGGGGGGGGGGGGGGG> %s"), *_namexx);
			UE_LOG(LogTemp, Log, TEXT("class -=========> %s"), *_classxx);




			//UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();



			//UMaterialInstanceDynamic* material = (UMaterialInstanceDynamic*)CreatedObject;
			//UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(NULL, CreatedObject);
			//CreatedObject = ma
			//material->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(1.0f, 0.0f, 0.0f));
			//UMaterialInstance* material = (UMaterialInstance*)CreatedObject;
			//CreatedObject = *material;
			//UMaterialInstance* material = (UMaterialInstance*)CreatedObject;

			//EditorAssetSubsystem->SetMetadataTag(CreatedObject, FName(TEXT("diffuse_color")), TEXT("{R:1.0, G:0.0, B:0.0}"));
			//EditorAssetSubsystem->SetMetadataTag(CreatedObject, FName(TEXT("base_color")), TEXT("{R:1.0, G:0.0, B:0.0}"));
			//EditorAssetSubsystem->SetMetadataTag(CreatedObject, FName(TEXT("BaseColor")), TEXT("{R:1.0, G:0.0, B:0.0}"));
			//EditorAssetSubsystem->SetMetadataTag(CreatedObject, FName(TEXT("DiffuseColor")), TEXT("{R:1.0, G:0.0, B:0.0}"));

			//FString _tag;
			//_tag = EditorAssetSubsystem->GetMetadataTag(CreatedObject, "BaseColor");
			//UE_LOG(LogTemp, Log, TEXT("TTTTTTTTTTTTTTTTTTTTTTTTT> %s"), *_tag);
			//_tag = EditorAssetSubsystem->GetMetadataTag(CreatedObject, "Color");
			//UE_LOG(LogTemp, Log, TEXT("TTTTTTTTTTTTTTTTTTTTTTTTT> %s"), *_tag);
			//_tag = EditorAssetSubsystem->GetMetadataTag(CreatedObject, "DiffuseColor");
			//UE_LOG(LogTemp, Log, TEXT("TTTTTTTTTTTTTTTTTTTTTTTTT> %s"), *_tag);
			//_tag = EditorAssetSubsystem->GetMetadataTag(CreatedObject, "base_color");
			//UE_LOG(LogTemp, Log, TEXT("TTTTTTTTTTTTTTTTTTTTTTTTT> %s"), *_tag);
			//_tag = EditorAssetSubsystem->GetMetadataTag(CreatedObject, "diffuse_color");
			//UE_LOG(LogTemp, Log, TEXT("TTTTTTTTTTTTTTTTTTTTTTTTT> %s"), *_tag);


			//CreatedObject->Modify();

			//UMaterial* material = (UMaterial*)CreatedObject;
			//material->Set


			//FString _class = CreatedObject->GetClass()->GetName(); // Material
			//UE_LOG(LogTemp, Log, TEXT("class -=========> %s"), *_class);
			//material->

			//FMaterialCompiler* xxxx = new FMaterialCompiler();
			UMaterialInstance* xxx = Cast<UMaterialInstance>(CreatedObject);

			UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(xxx, NULL);
			material->SetVectorParameterValue(FName(TEXT("BaseColor")), FLinearColor(1.0f, 0.0f, 0.0f));


			//FName _sname = CreatedObject->SourceFileTagName();
			//FAssetData asset;
			//TArray<UObject::FAssetRegistryTag> masset;
			//CreatedObject->GetAssetRegistryTags(asset);

			//for (auto& aaa : masset) {
				//aaa.ToSt

			//CreatedObject->Ass
			//UE_LOG(LogTemp, Log, TEXT("====================> %s"), *(masset.ToString()));



			//asset.key

			//UE_LOG(LogTemp, Log, TEXT("====================> %s"), *_sname.ToString());



			//FAssetData
			//CreatedObject->GetMate
			//UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(NULL, CreatedObject);




			//material->Modify();

			//material->SetVectorParameterValue(FName(TEXT("Base Color")), FLinearColor(1.0f, 0.0f, 0.0f));

			//CreatedObject->Modify();


			//EditorAssetSubsystem->SetMetadataTag(material, FName(TEXT("diffuse_color")), TEXT("{\"R\":1.0, \"G\":0.0, \"B\":0.0}"));
			//EditorAssetSubsystem->SetMetadataTag(material, FName(TEXT("Input DiffuseColor")), TEXT("{\"R\":1.0, \"G\":0.0, \"B\":0.0}"));
			//EditorAssetSubsystem->SetMetadataTag(material, FName(TEXT("Input DiffuseColor (Vector3)")), TEXT("{\"R\":1.0, \"G\":0.0, \"B\":0.0}"));
			//EditorAssetSubsystem->SetMetadataTag(material, FName(TEXT("base_color")), TEXT("{\"R\":1.0, \"G\":0.0, \"B\":0.0}"));
			//EditorAssetSubsystem->SetMetadataTag(material, FName(TEXT("BaseColor")), TEXT("{\"R\":1.0, \"G\":0.0, \"B\":0.0}"));

			//TMap<FName, FString> _maps = EditorAssetSubsystem->GetMetadataTagValues(material);
			//for (auto& aaa : _maps) {
			//	UE_LOG(LogTemp, Log, TEXT("xxxxxxxxxxxxxxx> %s  %s"), *(aaa.Key).ToString(), *aaa.Value);
			//}
		}*/
	}
	//UE_LOG(LogTemp, Log, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"));

}



void UMyEditorSubsystem::OnAssetPreImport(UFactory* Factory, UClass* InClass, UObject* CreatedObject, const FName& Name, const TCHAR* Type)
{
	return;
	/**/
	UE_LOG(LogTemp, Log, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX name %s"), *Name.ToString());
	UE_LOG(LogTemp, Log, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX type %s"), Type);

	if (Factory != NULL) {
		FString _class = Factory->GetClass()->GetName();
		UE_LOG(LogTemp, Log, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Factory %s"), *_class);
	}


	if (CreatedObject != NULL) {
		FString _class = CreatedObject->GetClass()->GetName();
		UE_LOG(LogTemp, Log, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Object %s"), *_class);
	}

	if (InClass != NULL) {
		FString _class = InClass->GetClass()->GetName();
		UE_LOG(LogTemp, Log, TEXT("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX class %s"), *_class);
	}
	/**/
}


void UMyEditorSubsystem::Deinitialize()
{
	if (GEditor != NULL)
	{
		UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>();
		if (ImportSubsystem != NULL)
		{
			ImportSubsystem->OnAssetPostImport.Remove(OnAssetPostImportHandle);
			ImportSubsystem->OnAssetPreImport.Remove(OnAssetPreImportHandle);
		}
	}
	Super::Deinitialize();
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
	params.Init(0.0, 24);
	params[3] = 1.0f;

	int32 _end_name = mtl_name.Find(TEXT("_"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, mtl_name.Len());
	FString _params_str = mtl_name.Right(mtl_name.Len() - _end_name - 1);
	_params_str = _params_str.Replace(TEXT("-"), TEXT("/"));
	UE_LOG(LogTemp, Log, TEXT("params = %s"),*_params_str);

	TArray<uint8> dec;
	FBase64::Decode(_params_str, dec);
	if (dec.Num() != 24) return params;

	params[0]  = 1.0f - (float)dec[0] / 255.0f;		// Red
	params[1]  = 1.0f - (float)dec[1] / 255.0f;		// Green
	params[2]  = 1.0f - (float)dec[2] / 255.0f;		// Blue
	params[3]  = 1.0f - (float)dec[3] / 255.0f;		// Alpha
	params[4]  = (float)dec[4] / 255.0f;			// CutOff
	params[5]  = (float)dec[5] / 255.0f;			// Shinuness
	params[6]  = (float)dec[6] / 255.0f;			// Glow
	params[7]  = (float)dec[7] / 255.0f;			// Bright
	params[8]  = (float)dec[8] / 255.0f;			// Light
	params[23] = (float)dec[23];					// Kind

	return params;
}
