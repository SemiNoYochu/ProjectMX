// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectMX/Public/Game/MXGameMode.h"

#include "Character/MXPlayerCharacter.h"

AMXGameMode::AMXGameMode()
{
	// /Script/Engine.Blueprint'/Game/ProjectMX/Character/BP_PlayerCharacter.BP_PlayerCharacter'
	ConstructorHelpers::FClassFinder<AMXPlayerCharacter> playerCharacter(TEXT("/Game/ProjectMX/Character/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (playerCharacter.Succeeded())
	{
		DefaultPawnClass = playerCharacter.Class;
	}
}
