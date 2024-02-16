// Fill out your copyright notice in the Description page of Project Settings.


#include "MyImportSubsystem.h"

void UMyImportSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Log, TEXT("UMyImportSubsystem::Initialize()"));
}

void UMyImportSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("UMyImportSubsystem::Deinitialize()"));
}



/*
void UMyImportSubsystem::OnAssetPreImportFunc(UFactory* InFactory, UClass* InClass, UObject* InParent, const FName& Name, const TCHAR* Type)
{
	UE_LOG(LogTemp, Log, TEXT("UMyImportSubsystem::OnAssetPreImportFunc()"));

}*/
/**/
void UMyImportSubsystem::OnAssetPostImportFunc(UFactory* InFactory, UObject* InCreatedObject)
{
	UE_LOG(LogTemp, Log, TEXT("UMyImportSubsystem::OnAssetPostImportFunc()"));
}

/*
void UMyImportSubsystem::FOnAssetReimportFunc(UObject* InCreatedObject)
{
	UE_LOG(LogTemp, Log, TEXT("UMyImportSubsystem::FOnAssetReimportFunc()"));

}

void UMyImportSubsystem::FOnAssetPostLODImportFunc(UObject* InObject, int32 inLODIndex)
{
	UE_LOG(LogTemp, Log, TEXT("UMyImportSubsystem::FOnAssetPostLODImportFunc()"));
}*/
