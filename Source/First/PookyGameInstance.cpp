// Fill out your copyright notice in the Description page of Project Settings.


#include "PookyGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "MainMenu.h"

UPookyGameInstance::UPookyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MainUI/WBP_MainUI"));

	MainMenu = MenuBPClass.Class;
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

void UPookyGameInstance::Play()
{	
	UWorld* World = GetWorld();
	if(Menu)
	{
		Menu->Teardown();
	}

	if(World)
	{
		World->ServerTravel("/Game/Maps/Test");
	}
}
