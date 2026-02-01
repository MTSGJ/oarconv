// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SetLocationByParameterStyle.h"

class FSetLocationByParameterCommands : public TCommands<FSetLocationByParameterCommands>
{
public:

	FSetLocationByParameterCommands()
		: TCommands<FSetLocationByParameterCommands>(TEXT("SetLocationByParameter"), NSLOCTEXT("Contexts", "SetLocationByParameter", "SetLocationByParameter Plugin"), NAME_None, FSetLocationByParameterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
