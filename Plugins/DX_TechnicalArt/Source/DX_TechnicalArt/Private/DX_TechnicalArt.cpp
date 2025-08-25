// Copyright Epic Games, Inc. All Rights Reserved.

#include "DX_TechnicalArt.h"

#define LOCTEXT_NAMESPACE "FDX_TechnicalArtModule"

void FDX_TechnicalArtModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDX_TechnicalArtModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDX_TechnicalArtModule, DX_TechnicalArt)