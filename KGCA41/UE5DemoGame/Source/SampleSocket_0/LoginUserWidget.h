// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLESOCKET_0_API ULoginUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent,Category = "Login")
		void LoginAck(const int& iRet);	
	
};
