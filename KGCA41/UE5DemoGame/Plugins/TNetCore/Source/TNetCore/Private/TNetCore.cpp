// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TNetCore.h"

#define LOCTEXT_NAMESPACE "FTNetCoreModule"

void FTNetCoreModule::StartupModule()
{
	//IPluginManager::Get().FindPlugin("PluginName")->GetBaseDir();
	//DLLHandle = FPlatformProcess::GetDllHandle(Path);
	//UE_LOG(LogTemp, Warning, TEXT("This text is  not printing :'C"));
}

void FTNetCoreModule::ShutdownModule()
{
	//*DLLHandle = FPlatformProcess::GetDllHandle(Path);
	//FPlatformProcess::FreeDllHandle(DLLHandle);
	//UE_LOG(LogTemp, Warning, TEXT("Log Ended"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTNetCoreModule, TNetCore)