// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BasicInputSystem/Public/BasicInputPlayerController.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBasicInputPlayerController() {}

// Begin Cross Module References
BASICINPUTSYSTEM_API UClass* Z_Construct_UClass_ABasicInputPlayerController();
BASICINPUTSYSTEM_API UClass* Z_Construct_UClass_ABasicInputPlayerController_NoRegister();
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UPackage* Z_Construct_UPackage__Script_BasicInputSystem();
// End Cross Module References

// Begin Class ABasicInputPlayerController
void ABasicInputPlayerController::StaticRegisterNativesABasicInputPlayerController()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABasicInputPlayerController);
UClass* Z_Construct_UClass_ABasicInputPlayerController_NoRegister()
{
	return ABasicInputPlayerController::StaticClass();
}
struct Z_Construct_UClass_ABasicInputPlayerController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n *\n */" },
#endif
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "BasicInputPlayerController.h" },
		{ "ModuleRelativePath", "Public/BasicInputPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultInputMapping_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/BasicInputPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_JumpAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/BasicInputPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MoveAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/BasicInputPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LookAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/BasicInputPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_InputVector_MetaData[] = {
		{ "Category", "BasicInputPlayerController" },
		{ "ModuleRelativePath", "Public/BasicInputPlayerController.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DefaultInputMapping;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_JumpAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MoveAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_LookAction;
	static const UECodeGen_Private::FStructPropertyParams NewProp_InputVector;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABasicInputPlayerController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_DefaultInputMapping = { "DefaultInputMapping", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABasicInputPlayerController, DefaultInputMapping), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultInputMapping_MetaData), NewProp_DefaultInputMapping_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_JumpAction = { "JumpAction", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABasicInputPlayerController, JumpAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_JumpAction_MetaData), NewProp_JumpAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_MoveAction = { "MoveAction", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABasicInputPlayerController, MoveAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MoveAction_MetaData), NewProp_MoveAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_LookAction = { "LookAction", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABasicInputPlayerController, LookAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LookAction_MetaData), NewProp_LookAction_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_InputVector = { "InputVector", nullptr, (EPropertyFlags)0x0010000000000014, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ABasicInputPlayerController, InputVector), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_InputVector_MetaData), NewProp_InputVector_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ABasicInputPlayerController_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_DefaultInputMapping,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_JumpAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_MoveAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_LookAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABasicInputPlayerController_Statics::NewProp_InputVector,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABasicInputPlayerController_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ABasicInputPlayerController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_BasicInputSystem,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABasicInputPlayerController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABasicInputPlayerController_Statics::ClassParams = {
	&ABasicInputPlayerController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ABasicInputPlayerController_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ABasicInputPlayerController_Statics::PropPointers),
	0,
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABasicInputPlayerController_Statics::Class_MetaDataParams), Z_Construct_UClass_ABasicInputPlayerController_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABasicInputPlayerController()
{
	if (!Z_Registration_Info_UClass_ABasicInputPlayerController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABasicInputPlayerController.OuterSingleton, Z_Construct_UClass_ABasicInputPlayerController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABasicInputPlayerController.OuterSingleton;
}
template<> BASICINPUTSYSTEM_API UClass* StaticClass<ABasicInputPlayerController>()
{
	return ABasicInputPlayerController::StaticClass();
}
ABasicInputPlayerController::ABasicInputPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABasicInputPlayerController);
ABasicInputPlayerController::~ABasicInputPlayerController() {}
// End Class ABasicInputPlayerController

// Begin Registration
struct Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABasicInputPlayerController, ABasicInputPlayerController::StaticClass, TEXT("ABasicInputPlayerController"), &Z_Registration_Info_UClass_ABasicInputPlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABasicInputPlayerController), 4134241664U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_1967245130(TEXT("/Script/BasicInputSystem"),
	Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
