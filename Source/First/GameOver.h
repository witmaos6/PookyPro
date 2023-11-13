// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "GameOver.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class FIRST_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ToMainButton;

	IMenuInterface* MenuInterface;

public:
	void SetMenuInterface(IMenuInterface* MenuInterfaceParam);

	void Setup();

	void TearDown();

protected:
	UFUNCTION()
	void ToMainMenu();
};
