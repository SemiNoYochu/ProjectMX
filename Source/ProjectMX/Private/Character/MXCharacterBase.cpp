// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MXCharacterBase.h"


// Sets default values
AMXCharacterBase::AMXCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

