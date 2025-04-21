// Copyright Epic Games, Inc. All Rights Reserved.

#include "DX_ReusableTool.h"

#define LOCTEXT_NAMESPACE "FDX_ReusableToolModule"

void FDX_ReusableToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDX_ReusableToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDX_ReusableToolModule, DX_ReusableTool)