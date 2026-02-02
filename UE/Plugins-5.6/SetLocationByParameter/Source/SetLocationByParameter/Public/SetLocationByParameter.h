/**
* Set Locationèàóù for OAR   by Fumi.Iseki
* 
*                     ver1.1.0  2024/02/26
*/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSetLocationByParameterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command. */
	void PluginButtonClicked();
	FVector GetLocationFromName(FString mtl_name, bool& rslt);

private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
