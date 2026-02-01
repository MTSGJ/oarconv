/**
* Set Location処理 for OAR   by Fumi.Iseki
*                               ver1.2.1
*/

#include "SetLocationByParameter.h"
#include "SetLocationByParameterStyle.h"
#include "SetLocationByParameterCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"


static const FName SetLocationByParameterTabName("SetLocationByParameter");


#define LOCATION_MAGIC_STR "metaverse_jp_net_"	// UE内で . は _ に変化する
#define LOCTEXT_NAMESPACE  "FSetLocationByParameterModule"

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
	UE_LOG(LogTemp, Log, TEXT("FSetLocationByParameterModule: Start"));

    if (!GEditor) {
        UE_LOG(LogTemp, Warning, TEXT("[SetLocationByParameter] GEditor is null"));
        return;
    }

	UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) {
        UE_LOG(LogTemp, Warning, TEXT("[SetLocationByParameter] World is null"));
        return;
    }

    // Undo対応
    const FScopedTransaction Transaction(NSLOCTEXT("SetLocationByParameter", "RelocateActors", "Relocate Actors"));
   
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), actors);

	for (AActor* act : actors) {
		AStaticMeshActor* SMA = Cast<AStaticMeshActor>(act);
		if (!SMA) {
			UE_LOG(LogTemp, Log, TEXT("[SetLocationByParameter] SMA is null"));
			continue;
		}

		UStaticMeshComponent* SMC = SMA->GetStaticMeshComponent();
		if (!SMC) continue;

		UStaticMesh* Mesh = SMC->GetStaticMesh();
		if (!Mesh) continue;

		
		bool rslt = false;
		FVector NewLoc = GetLocationFromName(Mesh->GetName(), rslt);
		if (!rslt) {
			rslt = false;
			NewLoc = GetLocationFromName(Mesh->GetOutermost()->GetName(), rslt);
		}

		if (rslt && NewLoc != act->GetActorLocation()) {
			act->Modify();			// Editorで変更として残す
			act->SetActorLocation(NewLoc);
			UE_LOG(LogTemp, Log, TEXT("[SetLocationByParameter] %s -> (%f, %f, %f)"), *Mesh->GetName(), NewLoc.X, NewLoc.Y, NewLoc.Z);
		}
	}

	GEditor->RedrawAllViewports(); // 表示更新
	UE_LOG(LogTemp, Log, TEXT("FSetLocationByParameterModule: End"));

	return;
}


FVector FSetLocationByParameterModule::GetLocationFromName(FString mesh_name, bool& rslt)
{
	rslt = false;
	FVector location(0.0, 0.0, 0.0);

	int32 _name_end = mesh_name.Find(TEXT(LOCATION_MAGIC_STR), ESearchCase::CaseSensitive, ESearchDir::FromEnd, mesh_name.Len());
	if (_name_end == -1) {
		UE_LOG(LogTemp, Log, TEXT("[SetLocationByParameter] err 1"))
		return location;
	}

	FString _magic_str = FString(TEXT(LOCATION_MAGIC_STR));
	int32 _magic_len = _magic_str.Len();
	//
	FString params_str = mesh_name.Right(mesh_name.Len() - _name_end - _magic_len);
	int32 param_size = sizeof(float) * 4; // sizeof(float)*3*8/6
	if (params_str.Len() != param_size) {
		params_str = params_str.Left(param_size);
	}
	params_str = params_str.Replace(TEXT("-"), TEXT("/"));
	//UE_LOG(LogTemp, Log, TEXT("Base64 Encode Data = %s"), *params_str);

	TArray<uint8> dec;
	try {
		FBase64::Decode(params_str, dec);
	}
	catch (...) {
		UE_LOG(LogTemp, Log, TEXT("Base64 Decode Error!"));
		return location;
	}
	int32 dec_size = param_size / 4 * 3;
	if (dec.Num() != dec_size) {
	    UE_LOG(LogTemp, Log, TEXT("[SetLocationByParameter] err 2"))
		return location;
	}

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
