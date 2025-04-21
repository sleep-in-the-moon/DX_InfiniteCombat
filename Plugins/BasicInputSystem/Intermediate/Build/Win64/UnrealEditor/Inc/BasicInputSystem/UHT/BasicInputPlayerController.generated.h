// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BasicInputPlayerController.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BASICINPUTSYSTEM_BasicInputPlayerController_generated_h
#error "BasicInputPlayerController.generated.h already included, missing '#pragma once' in BasicInputPlayerController.h"
#endif
#define BASICINPUTSYSTEM_BasicInputPlayerController_generated_h

#define FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_20_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesABasicInputPlayerController(); \
	friend struct Z_Construct_UClass_ABasicInputPlayerController_Statics; \
public: \
	DECLARE_CLASS(ABasicInputPlayerController, APlayerController, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/BasicInputSystem"), NO_API) \
	DECLARE_SERIALIZER(ABasicInputPlayerController)


#define FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_20_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ABasicInputPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ABasicInputPlayerController(ABasicInputPlayerController&&); \
	ABasicInputPlayerController(const ABasicInputPlayerController&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ABasicInputPlayerController); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ABasicInputPlayerController); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ABasicInputPlayerController) \
	NO_API virtual ~ABasicInputPlayerController();


#define FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_17_PROLOG
#define FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_20_INCLASS_NO_PURE_DECLS \
	FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h_20_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BASICINPUTSYSTEM_API UClass* StaticClass<class ABasicInputPlayerController>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Dev_X_UE_Project_UE5_5_DX_InfiniteCombat_Plugins_BasicInputSystem_Source_BasicInputSystem_Public_BasicInputPlayerController_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
