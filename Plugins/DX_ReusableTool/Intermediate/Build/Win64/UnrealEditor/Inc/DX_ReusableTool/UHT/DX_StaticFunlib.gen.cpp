// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "DX_ReusableTool/Public/DX_StaticFunlib.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDX_StaticFunlib() {}

// Begin Cross Module References
DX_REUSABLETOOL_API UClass* Z_Construct_UClass_UDX_StaticFunlib();
DX_REUSABLETOOL_API UClass* Z_Construct_UClass_UDX_StaticFunlib_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
UPackage* Z_Construct_UPackage__Script_DX_ReusableTool();
// End Cross Module References

// Begin Class UDX_StaticFunlib
void UDX_StaticFunlib::StaticRegisterNativesUDX_StaticFunlib()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UDX_StaticFunlib);
UClass* Z_Construct_UClass_UDX_StaticFunlib_NoRegister()
{
	return UDX_StaticFunlib::StaticClass();
}
struct Z_Construct_UClass_UDX_StaticFunlib_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "DX_StaticFunlib.h" },
		{ "ModuleRelativePath", "Public/DX_StaticFunlib.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDX_StaticFunlib>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UDX_StaticFunlib_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_DX_ReusableTool,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDX_StaticFunlib_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UDX_StaticFunlib_Statics::ClassParams = {
	&UDX_StaticFunlib::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UDX_StaticFunlib_Statics::Class_MetaDataParams), Z_Construct_UClass_UDX_StaticFunlib_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UDX_StaticFunlib()
{
	if (!Z_Registration_Info_UClass_UDX_StaticFunlib.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDX_StaticFunlib.OuterSingleton, Z_Construct_UClass_UDX_StaticFunlib_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UDX_StaticFunlib.OuterSingleton;
}
template<> DX_REUSABLETOOL_API UClass* StaticClass<UDX_StaticFunlib>()
{
	return UDX_StaticFunlib::StaticClass();
}
UDX_StaticFunlib::UDX_StaticFunlib(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UDX_StaticFunlib);
UDX_StaticFunlib::~UDX_StaticFunlib() {}
// End Class UDX_StaticFunlib

// Begin Registration
struct Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_DX_ReusableTool_Source_DX_ReusableTool_Public_DX_StaticFunlib_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UDX_StaticFunlib, UDX_StaticFunlib::StaticClass, TEXT("UDX_StaticFunlib"), &Z_Registration_Info_UClass_UDX_StaticFunlib, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDX_StaticFunlib), 1320633943U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_DX_ReusableTool_Source_DX_ReusableTool_Public_DX_StaticFunlib_h_3712023093(TEXT("/Script/DX_ReusableTool"),
	Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_DX_ReusableTool_Source_DX_ReusableTool_Public_DX_StaticFunlib_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_DX_ReusableTool_Source_DX_ReusableTool_Public_DX_StaticFunlib_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
