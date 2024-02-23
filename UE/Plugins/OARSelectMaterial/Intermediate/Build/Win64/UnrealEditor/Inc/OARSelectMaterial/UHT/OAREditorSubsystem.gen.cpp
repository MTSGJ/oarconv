// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OARSelectMaterial/Public/OAREditorSubsystem.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOAREditorSubsystem() {}
// Cross Module References
	EDITORSUBSYSTEM_API UClass* Z_Construct_UClass_UEditorSubsystem();
	OARSELECTMATERIAL_API UClass* Z_Construct_UClass_UOAREditorSubsystem();
	OARSELECTMATERIAL_API UClass* Z_Construct_UClass_UOAREditorSubsystem_NoRegister();
	UPackage* Z_Construct_UPackage__Script_OARSelectMaterial();
// End Cross Module References
	void UOAREditorSubsystem::StaticRegisterNativesUOAREditorSubsystem()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UOAREditorSubsystem);
	UClass* Z_Construct_UClass_UOAREditorSubsystem_NoRegister()
	{
		return UOAREditorSubsystem::StaticClass();
	}
	struct Z_Construct_UClass_UOAREditorSubsystem_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UOAREditorSubsystem_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UEditorSubsystem,
		(UObject* (*)())Z_Construct_UPackage__Script_OARSelectMaterial,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOAREditorSubsystem_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UOAREditorSubsystem_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * OARSELECTMATERIAL_API\n */" },
		{ "IncludePath", "OAREditorSubsystem.h" },
		{ "ModuleRelativePath", "Public/OAREditorSubsystem.h" },
		{ "ToolTip", "OARSELECTMATERIAL_API" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UOAREditorSubsystem_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOAREditorSubsystem>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UOAREditorSubsystem_Statics::ClassParams = {
		&UOAREditorSubsystem::StaticClass,
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
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UOAREditorSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UOAREditorSubsystem_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UOAREditorSubsystem()
	{
		if (!Z_Registration_Info_UClass_UOAREditorSubsystem.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UOAREditorSubsystem.OuterSingleton, Z_Construct_UClass_UOAREditorSubsystem_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UOAREditorSubsystem.OuterSingleton;
	}
	template<> OARSELECTMATERIAL_API UClass* StaticClass<UOAREditorSubsystem>()
	{
		return UOAREditorSubsystem::StaticClass();
	}
	UOAREditorSubsystem::UOAREditorSubsystem() {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UOAREditorSubsystem);
	UOAREditorSubsystem::~UOAREditorSubsystem() {}
	struct Z_CompiledInDeferFile_FID_HostProject_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UOAREditorSubsystem, UOAREditorSubsystem::StaticClass, TEXT("UOAREditorSubsystem"), &Z_Registration_Info_UClass_UOAREditorSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOAREditorSubsystem), 1750384086U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_HostProject_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_1097232776(TEXT("/Script/OARSelectMaterial"),
		Z_CompiledInDeferFile_FID_HostProject_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_HostProject_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
