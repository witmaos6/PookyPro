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
#include "PookyGameInstance.h"


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
	bMusicPlayTemp = false;

	ThirdSkillSpeed = 900.f;
	ThirdSkillDelta = 1.5;
	ThirdSkillTime = 5.0f;
}

void ARunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FString CurrentLevel = World->GetMapName();
	if (CurrentLevel == "UEDPIE_0_BeginLevel") // UEDPIE_0_ 는 언리얼에서 붙이는 수식어 인듯 하다.
	{
		AudioComponent->Stop();
		return;
	}
	bMusicPlayTemp = true;

	Runner = Cast<ARunner>(GetCharacter());
	ThirdSkillSpeed = Runner->GetMovementComponent()->GetMaxSpeed() * ThirdSkillDelta;

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
	if(Runner->GetCharacterState() == ECharacterState::ECS_Normal && bRailsChangeable && CurrentRail < RightRail)
	{
		bRailsChangeable = false;

		CurrentRail++;

		ChangeRail(1.0f);
	}
}

void ARunnerPlayerController::MoveLeft(const FInputActionValue& Value)
{
	if (Runner->GetCharacterState() == ECharacterState::ECS_Normal && bRailsChangeable && CurrentRail > LeftRail)
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
	if(Runner && Runner->GetCharacterState() == ECharacterState::ECS_Normal)
	{
		UCharacterMovementComponent* Movement = Runner->GetCharacterMovement();
		if(Movement)
		{
			UAnimInstance* AnimInstance = Runner->GetMesh()->GetAnimInstance();
			if (AnimInstance && Runner->GetSlideMontage())
			{
				Runner->SetCharacterState(ECharacterState::ECS_Slide);

				int32 Index = FMath::RandRange(0, 1);
				FName SectionName = (Index == 0) ? "FowardRoll" : "RunningSlide";
				AnimInstance->Montage_Play(Runner->GetSlideMontage(), 1.0f);
				AnimInstance->Montage_JumpToSection(SectionName, Runner->GetSlideMontage());
			}
		}
	}
}

void ARunnerPlayerController::Jump(const FInputActionValue& Value)
{
	if(Runner && (Runner->GetCharacterState() == ECharacterState::ECS_Normal || Runner->GetCharacterState() == ECharacterState::ECS_Jump))
	{
		Runner->Jump();
		Runner->SetCharacterState(ECharacterState::ECS_Jump);
	}
	GetWorldTimerManager().SetTimer(JumpTimer, this, &ARunnerPlayerController::ResetJumpState, GetWorld()->GetDeltaSeconds(), true);
}

void ARunnerPlayerController::ResetJumpState()
{
	if(!Runner->GetMovementComponent()->IsFalling())
	{
		Runner->SetCharacterState(ECharacterState::ECS_Normal);
		GetWorldTimerManager().ClearTimer(JumpTimer);
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
		bMusicPlayTemp = true;
		AudioComponent->SetSound(BackGroundMusic);
		AudioComponent->Play();
	}
}

void ARunnerPlayerController::OpenGameOver()
{
	UPookyGameInstance* GameInstance = Cast<UPookyGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		GameInstance->LoadGameOverUI();
	}
}

void ARunnerPlayerController::BGMPitchUp()
{
	if (BackGroundMusic)
	{
		AudioComponent->SetPitchMultiplier(ThirdSkillDelta);
		Runner->GetCharacterMovement()->MaxWalkSpeed = ThirdSkillSpeed;

		FTimerHandle ResetTimer;
		GetWorldTimerManager().SetTimer(ResetTimer, this, &ARunnerPlayerController::BGMPitchReset, ThirdSkillTime, false);
	}
}

void ARunnerPlayerController::BGMPitchReset()
{
	AudioComponent->SetPitchMultiplier(1.0f);
	Runner->GetCharacterMovement()->MaxWalkSpeed = Runner->GetBasicSpeed();
}

void ARunnerPlayerController::SpawnHaetaeAndPossess()
{
	if(Haetae)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		HaetaeCharacter = GetWorld()->SpawnActor<ACharacter>(Haetae, Runner->GetActorLocation(), Runner->GetActorRotation(), SpawnParameters);
		Possess(HaetaeCharacter);

		GetWorldTimerManager().SetTimer(ThirdSkillTimer, this, &ARunnerPlayerController::ResetPossess, ThirdSkillTime);
	}
}

void ARunnerPlayerController::ResetPossess()
{
	Runner->SetActorTransform(HaetaeCharacter->GetActorTransform());
	Possess(Runner);
	Runner->SetCharacterState(ECharacterState::ECS_Normal);
	HaetaeCharacter->Destroy();
}
