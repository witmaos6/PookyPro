// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver.h"

#include "RunnerPlayerController.h"
#include "Components/Button.h"

bool UGameOver::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
	{
		return false;
	}

	ToMainButton->OnClicked.AddDynamic(this, &UGameOver::ToMainMenu);

	return true;
}

void UGameOver::SetMenuInterface(IMenuInterface* MenuInterfaceParam)
{
	MenuInterface = MenuInterfaceParam;
}

void UGameOver::Setup()
{
	this->AddToViewport();

	ARunnerPlayerController* PlayerController = Cast<ARunnerPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		FInputModeUIOnly InputModeData;
		SetIsFocusable(true);
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}
}

void UGameOver::TearDown()
{
	this->RemoveFromParent();

	ARunnerPlayerController* PlayerController = Cast<ARunnerPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		FInputModeUIOnly InputModeData;
		SetIsFocusable(true);
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;
	}
}

void UGameOver::ToMainMenu()
{
	if (MenuInterface)
	{
		MenuInterface->ToMainMenu();
	}
}
