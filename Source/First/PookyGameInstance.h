// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "PookyGameInstance.generated.h"

class UMainMenu;
class UGameOver;
/**
 * 
 */
UCLASS()
class FIRST_API UPookyGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UPookyGameInstance(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> MainMenu;

	UMainMenu* Menu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> GameOverUI;

	UGameOver* GameOver;

public:
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadGameOverUI();

	UFUNCTION(Exec)
	virtual void Play() override;

	UFUNCTION(Exec)
	virtual void ToMainMenu() override;
};
