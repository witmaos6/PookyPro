// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "RunnerPlayerController.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
	{
		return false;
	}

	PlayButton->OnClicked.AddDynamic(this, &UMainMenu::Play);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterfaceParam)
{
	MenuInterface = MenuInterfaceParam;
}

void UMainMenu::Setup()
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

void UMainMenu::Teardown()
{
	this->RemoveFromParent();

	ARunnerPlayerController* PlayerController = Cast<ARunnerPlayerController>(GetWorld()->GetFirstPlayerController());
	FInputModeGameOnly InputModeData;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::Play()
{
	if(MenuInterface)
	{
		MenuInterface->Play();
	}
}
