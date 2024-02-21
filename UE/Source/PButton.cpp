// Copyright Epic Games, Inc. All Rights Reserved.

#include "PButton.h"
#include "PButtonStyle.h"
#include "PButtonCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include <Kismet/GameplayStatics.h>
//#include <Particles/Emitter.h>
#include "Engine/StaticMeshActor.h"

static const FName PButtonTabName("PButton");

#define LOCTEXT_NAMESPACE "FPButtonModule"

void FPButtonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FPButtonStyle::Initialize();
	FPButtonStyle::ReloadTextures();

	FPButtonCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPButtonCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FPButtonModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPButtonModule::RegisterMenus));
}

void FPButtonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FPButtonStyle::Shutdown();

	FPButtonCommands::Unregister();
}

void FPButtonModule::PluginButtonClicked()
{
	TArray<AActor*> actors;
	UWorld* World = GEditor->GetEditorWorldContext().World();
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), actors); 
	for (AActor* act : actors) {
		FString class_name = act->GetClass()->GetName();
		if (class_name.Find(TEXT("StaticMeshActor")) == 0) {
			UStaticMeshComponent* smhc = Cast<AStaticMeshActor>(act)->GetStaticMeshComponent();
			if (smhc != NULL) {
				UStaticMesh* mesh = smhc->GetStaticMesh();
				if (mesh != NULL) {
					FString mesh_name = mesh->GetName();
					FVector vv = GetLocationFromName(mesh_name);
					if (vv != FVector(0.0, 0.0, 0.0)) {
						
						vv += act->GetActorLocation();
						/*
						vv.X = vv.X + vc.X;
						vv.Y = vv.Y + vc.Y;
						vv.Z = vv.Z + vc.Z;*/
						act->SetActorLocation(vv);
						UE_LOG(LogTemp, Log, TEXT("Actor %s is relocated to (%f, %f, %f)"), *mesh_name, vv.X, vv.Y, vv.Z);
					}
				}
			}
		}
	}
	return;
}


FVector FPButtonModule::GetLocationFromName(FString mesh_name)
{
	FVector location(0.0, 0.0, 0.0);

	int32 _name_end = mesh_name.Find(TEXT("_"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, mesh_name.Len());
	FString params_str = mesh_name.Right(mesh_name.Len() - _name_end - 1);
	int32 param_size = sizeof(float) * 4;
	if (params_str.Len() != param_size) return location;
	params_str = params_str.Replace(TEXT("-"), TEXT("/"));

	TArray<uint8> dec;
	FBase64::Decode(params_str, dec);
	int32 dec_size = param_size / 4 * 3;
	if (dec.Num() != dec_size) return location;

	float position[3];
	memcpy(position, dec.GetData(), dec_size);
	location.Set(position[0], position[1], position[2]);
	return location;
}


void FPButtonModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FPButtonCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FPButtonCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPButtonModule, PButton)