// Fill out your copyright notice in the Description page of Project Settings.


#include "TurningPoint.h"

#include "Runner.h"
#include "Components/BoxComponent.h"

// Sets default values
ATurningPoint::ATurningPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	Angle = 30.0f;
}

// Called when the game starts or when spawned
void ATurningPoint::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATurningPoint::OnOverlapBegin);
}

// Called every frame
void ATurningPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurningPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunner* Runner = Cast<ARunner>(OtherActor);

	if(Runner)
	{
		Runner->Turning(Angle);
	}
}
