// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PookyGameMode.generated.h"

class USoundCue;
/**
 * 
 */
UCLASS()
class FIRST_API APookyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APookyGameMode();


protected:
	// To do: 레벨에 맞는 음악 제공 모듈 추가
};
