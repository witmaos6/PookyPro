// Fill out your copyright notice in the Description page of Project Settings.


#include "PookyGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "MainMenu.h"
#include "GameOver.h"


UPookyGameInstance::UPookyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MainUI/WBP_MainUI"));
	MainMenu = MenuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverBPClass(TEXT("/Game/MainUI/WBP_GameOver"));
	GameOverUI = GameOverBPClass.Class;
}

void UPookyGameInstance::Init()
{

}

void UPookyGameInstance::LoadMenu()
{
	if(MainMenu)
	{
		Menu = CreateWidget<UMainMenu>(this, MainMenu);
		Menu->Setup();
		Menu->SetMenuInterface(this);
	}
}

void UPookyGameInstance::LoadGameOverUI()
{
	if (GameOverUI)
	{
		GameOver = CreateWidget<UGameOver>(this, GameOverUI);
		GameOver->Setup();
		GameOver->SetMenuInterface(this);
	}
}

void UPookyGameInstance::Play()
{	
	if(Menu)
	{
		Menu->Teardown();
	}

	UWorld* World = GetWorld();
	if(World)
	{
		World->ServerTravel("/Game/Maps/Test");
	}
}

void UPookyGameInstance::ToMainMenu()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/MainUI/BeginLevel");
	}
}
