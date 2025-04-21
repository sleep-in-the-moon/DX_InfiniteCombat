// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BasicInputSystem/Public/BasicCharacter.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBasicCharacter() {}

// Begin Cross Module References
BASICINPUTSYSTEM_API UClass* Z_Construct_UClass_ABasicCharacter();
BASICINPUTSYSTEM_API UClass* Z_Construct_UClass_ABasicCharacter_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_ACharacter();
ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USpringArmComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_BasicInputSystem();
// End Cross Module References

// Begin Class ABasicCharacter
void ABasicCharacter::StaticRegisterNativesABasicCharacter()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABasicCharacter);
UClass* Z_Construct_UClass_ABasicCharacter_NoRegister()
{
	return ABasicCharacter::StaticClass();
}
struct Z_Construct_UClass_ABasicCharacter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "BasicCharacter.h" },
		{ "ModuleRelativePath", "Public/BasicCharacter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SpringArm_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Camera" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/BasicCharacter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Camera_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Camera" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/BasicCharacter.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SpringArm;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Camera;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABasicCharacter>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABasicCharacter_Statics::NewProp_SpringArm = { "SpringArm", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABasicCharacter, SpringArm), Z_Construct_UClass_USpringArmComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SpringArm_MetaData), NewProp_SpringArm_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABasicCharacter_Statics::NewProp_Camera = { "Camera", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABasicCharacter, Camera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Camera_MetaData), NewProp_Camera_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ABasicCharacter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABasicCharacter_Statics::NewProp_SpringArm,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABasicCharacter_Statics::NewProp_Camera,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABasicCharacter_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ABasicCharacter_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACharacter,
	(UObject* (*)())Z_Construct_UPackage__Script_BasicInputSystem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABasicCharacter_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABasicCharacter_Statics::ClassParams = {
	&ABasicCharacter::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ABasicCharacter_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ABasicCharacter_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABasicCharacter_Statics::Class_MetaDataParams), Z_Construct_UClass_ABasicCharacter_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABasicCharacter()
{
	if (!Z_Registration_Info_UClass_ABasicCharacter.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABasicCharacter.OuterSingleton, Z_Construct_UClass_ABasicCharacter_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABasicCharacter.OuterSingleton;
}
template<> BASICINPUTSYSTEM_API UClass* StaticClass<ABasicCharacter>()
{
	return ABasicCharacter::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABasicCharacter);
ABasicCharacter::~ABasicCharacter() {}
// End Class ABasicCharacter

// Begin Registration
struct Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicCharacter_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABasicCharacter, ABasicCharacter::StaticClass, TEXT("ABasicCharacter"), &Z_Registration_Info_UClass_ABasicCharacter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABasicCharacter), 2726561988U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicCharacter_h_3033949752(TEXT("/Script/BasicInputSystem"),
	Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicCharacter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicCharacter_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
