// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/ImportSubsystem.h"
#include "MyImportSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyImportSubsystem : public UImportSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();

	//virtual void OnAssetPreImportFunc(UFactory* InFactory, UClass* InClass, UObject* InParent, const FName& Name, const TCHAR* Type);
	//virtual void OnAssetPostImportFunc(UFactory* InFactory, UObject* InCreatedObject);
	//virtual void FOnAssetReimportFunc(UObject* InCreatedObject);
	//virtual void FOnAssetPostLODImportFunc(UObject* InObject, int32 inLODIndex);
};
