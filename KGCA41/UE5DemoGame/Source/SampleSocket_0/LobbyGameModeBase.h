// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TGameMode.h"
#include "LobbyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLESOCKET_0_API ALobbyGameModeBase : public ATGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		class UUserWidget* pChatWiz;

	UFUNCTION(BlueprintCallable, Category = "Chat")
		void ChatToServer(FString Text);

	void UpdateChat(const string chat);
	class AGamePlayerController*  m_pPlayerController;
public:
	virtual void BeginPlay() override;
	virtual void ProcessPacket();
public:
	ALobbyGameModeBase();
};
