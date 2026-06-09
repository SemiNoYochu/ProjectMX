// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectMX/Public/Character/MXCharacter.h"


// Sets default values
AMXCharacter::AMXCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMXCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

