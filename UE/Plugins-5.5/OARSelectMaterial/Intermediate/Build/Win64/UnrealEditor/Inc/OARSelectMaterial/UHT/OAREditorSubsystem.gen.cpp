// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "OARSelectMaterial/Public/OAREditorSubsystem.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeOAREditorSubsystem() {}

// Begin Cross Module References
EDITORSUBSYSTEM_API UClass* Z_Construct_UClass_UEditorSubsystem();
OARSELECTMATERIAL_API UClass* Z_Construct_UClass_UOAREditorSubsystem();
OARSELECTMATERIAL_API UClass* Z_Construct_UClass_UOAREditorSubsystem_NoRegister();
UPackage* Z_Construct_UPackage__Script_OARSelectMaterial();
// End Cross Module References

// Begin Class UOAREditorSubsystem
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
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * OARSELECTMATERIAL_API\n */" },
#endif
		{ "IncludePath", "OAREditorSubsystem.h" },
		{ "ModuleRelativePath", "Public/OAREditorSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "OARSELECTMATERIAL_API" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UOAREditorSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UOAREditorSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEditorSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_OARSelectMaterial,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UOAREditorSubsystem_Statics::DependentSingletons) < 16);
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
// End Class UOAREditorSubsystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_MyProject21_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UOAREditorSubsystem, UOAREditorSubsystem::StaticClass, TEXT("UOAREditorSubsystem"), &Z_Registration_Info_UClass_UOAREditorSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UOAREditorSubsystem), 4268562365U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_MyProject21_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_3096789551(TEXT("/Script/OARSelectMaterial"),
	Z_CompiledInDeferFile_FID_MyProject21_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_MyProject21_Plugins_OARSelectMaterial_Source_OARSelectMaterial_Public_OAREditorSubsystem_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
