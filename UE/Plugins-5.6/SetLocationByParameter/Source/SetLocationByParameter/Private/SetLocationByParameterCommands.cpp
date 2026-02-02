// Copyright Epic Games, Inc. All Rights Reserved.

#include "SetLocationByParameterCommands.h"

#define LOCTEXT_NAMESPACE "FSetLocationByParameterModule"

void FSetLocationByParameterCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SetLocationByParameter", "Execute SetLocationByParameter action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
