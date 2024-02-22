// Copyright Epic Games, Inc. All Rights Reserved.

#include "SetLocationByParameter.h"
#include "SetLocationByParameterStyle.h"
#include "SetLocationByParameterCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"


static const FName SetLocationByParameterTabName("SetLocationByParameter");

#define LOCTEXT_NAMESPACE "FSetLocationByParameterModule"

void FSetLocationByParameterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSetLocationByParameterStyle::Initialize();
	FSetLocationByParameterStyle::ReloadTextures();

	FSetLocationByParameterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSetLocationByParameterCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSetLocationByParameterModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSetLocationByParameterModule::RegisterMenus));
}


void FSetLocationByParameterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSetLocationByParameterStyle::Shutdown();

	FSetLocationByParameterCommands::Unregister();
}


void FSetLocationByParameterModule::PluginButtonClicked()
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
					bool rslt = false;
					FVector vv = GetLocationFromName(mesh_name, rslt);
					if (rslt && vv!= act->GetActorLocation()) {
						act->SetActorLocation(vv);
						UE_LOG(LogTemp, Log, TEXT("FSetLocationByParameterModule: Actor %s is relocated to (%lf, %lf, %lf)"), 
							*mesh_name, vv.X, vv.Y, vv.Z);
					}
				}
			}
		}
	}
	return;
}


FVector FSetLocationByParameterModule::GetLocationFromName(FString mesh_name, bool& rslt)
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
	rslt = true;
	return location;
}


void FSetLocationByParameterModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSetLocationByParameterCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSetLocationByParameterCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSetLocationByParameterModule, SetLocationByParameter)
