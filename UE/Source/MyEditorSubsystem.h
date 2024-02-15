// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "MyEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API UMyEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	

public:
	// Begin UEditorSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End UEditorSubsystem Interface


	FDelegateHandle OnAssetPostImportHandle;
	FDelegateHandle OnAssetPreImportHandle;

	//UPROPERTY(Transient)
	void OnAssetPostImport(UFactory* Factory, UObject* CreatedObject);
	void OnAssetPreImport(UFactory* InFactory, UClass* InClass, UObject* InParent, const FName& Name, const TCHAR* Type);


	FString GetTexturePath(FString mtl_path, FString mtl_name);

	TArray<float> GetTextureParams(FString mtl_name);
};
