// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPlayerController.h"

#include "InputDataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Runner.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundBase.h"

#include "Components/AudioComponent.h"


ARunnerPlayerController::ARunnerPlayerController(const FObjectInitializer& ObjectInitializer)
{
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	MoveInterval = 200.f;
	bRailsChangeable = true;

	LeftRail = 0;
	CurrentRail = 1;
	RightRail = 2;

	TimelineBegin = 0.0f;
	TimelineEnd = 1.0f;

	MusicPlayTime = 0.0f;
	bMusicPlayTemp = true;
}

void ARunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FString CurrentLevel = World->GetMapName();
	if (CurrentLevel == "UEDPIE_0_BeginLevel") // UEDPIE_0_ 는 언리얼에서 붙이는 수식어 인듯 하다.
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, CurrentLevel);

	Runner = Cast<ARunner>(GetCharacter());

	if(Runner)
	{
		TimelineUpdateDelegate.BindDynamic(this, &ARunnerPlayerController::TimelineUpdateFunc);
		TimelineFinishedDelegate.BindDynamic(this, &ARunnerPlayerController::TimelineFinishedFunc);
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedDelegate);
		TimelineComponent->SetLooping(false);

		if (TimelineCurve)
		{
			TimelineComponent->AddInterpFloat(TimelineCurve, TimelineUpdateDelegate);
			TimelineCurve->GetTimeRange(TimelineBegin, TimelineEnd);
			TimelineComponent->SetTimelineLength(TimelineEnd);
		}

		MusicStart();

		if (WBPCharacterStatus)
		{
			CharacterStatus = CreateWidget<UUserWidget>(this, WBPCharacterStatus);
			CharacterStatus->AddToViewport();
			CharacterStatus->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ARunnerPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bMusicPlayTemp)
	{
		MusicPlayTime += DeltaSeconds;
	}
}

void ARunnerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInput->BindAction(InputData->MoveRight, ETriggerEvent::Triggered, this, &ARunnerPlayerController::MoveRight);
	EnhancedInput->BindAction(InputData->MoveLeft, ETriggerEvent::Triggered, this, &ARunnerPlayerController::MoveLeft);
	EnhancedInput->BindAction(InputData->Jump, ETriggerEvent::Triggered, this, &ARunnerPlayerController::Jump);
	EnhancedInput->BindAction(InputData->Slide , ETriggerEvent::Triggered, this, &ARunnerPlayerController::Slide);
}

void ARunnerPlayerController::MoveRight(const FInputActionValue& Value)
{
	if(!Runner->GetCharacterMovement()->IsFalling() && bRailsChangeable && CurrentRail < RightRail)
	{
		bRailsChangeable = false;

		CurrentRail++;

		ChangeRail(1.0f);
	}
}

void ARunnerPlayerController::MoveLeft(const FInputActionValue& Value)
{
	if (!Runner->GetCharacterMovement()->IsFalling() && bRailsChangeable && CurrentRail > LeftRail)
	{
		bRailsChangeable = false;

		CurrentRail--;

		ChangeRail(-1.0f);
	}
}

void ARunnerPlayerController::ChangeRail(float Direction)
{
	FVector ForwardVector = Runner->GetActorForwardVector() * Runner->GetDiagonal();
	FVector RightVector = Runner->GetActorRightVector() * Direction * MoveInterval;
	TimelineBeginLocation = Runner->GetActorLocation();
	TimelineEndLocation = TimelineBeginLocation + ForwardVector + RightVector;

	TimelineComponent->PlayFromStart();
}

void ARunnerPlayerController::TimelineUpdateFunc(float Output)
{
	FVector Location = FMath::Lerp(TimelineBeginLocation, TimelineEndLocation, Output);

	Runner->SetActorLocation(Location);
}

void ARunnerPlayerController::TimelineFinishedFunc()
{
	bRailsChangeable = true;
}

void ARunnerPlayerController::Slide(const FInputActionValue& Value)
{
	if(Runner && !Runner->GetMovementComponent()->IsFalling())
	{
		UCharacterMovementComponent* Movement = Runner->GetCharacterMovement();
		if(Movement)
		{
				//UAnimInstance* AnimInstance = Runner->GetMesh()->GetAnimInstance();
				//To do: MontagePlay and Ignore Collision process
		}
	}
}

void ARunnerPlayerController::Jump(const FInputActionValue& Value)
{
	if(Runner)
	{
		Runner->Jump();
		// To do: 2단 점프
	}
}

void ARunnerPlayerController::BGMStop()
{
	bMusicPlayTemp = false;

	AudioComponent->Stop();
}

void ARunnerPlayerController::BGMPlay()
{
	bMusicPlayTemp = true;

	AudioComponent->Play(MusicPlayTime);

	RaiseSound();
}

void ARunnerPlayerController::MusicStart()
{
	if (BackGroundMusic)
	{
		AudioComponent->SetSound(BackGroundMusic);
		AudioComponent->Play();
	}
}

void ARunnerPlayerController::OpenGameOver()
{
	
}
