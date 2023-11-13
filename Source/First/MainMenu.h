// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class FIRST_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

	IMenuInterface* MenuInterface;

public:
	void SetMenuInterface(IMenuInterface* MenuInterfaceParam);

	void Setup();

	void Teardown();

protected:
	UFUNCTION()
	void Play();
};
