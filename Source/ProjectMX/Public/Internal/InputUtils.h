#pragma once

#include "CoreMinimal.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

namespace InputUtils
{
    inline void AddInputMapping(APawn* Pawn, const UInputMappingContext* MappingContext, int32 Priority = 0)
    {
        if (ensure(IsValid(Pawn)) == false)
        {
            return;
        }

        if (ensure(IsValid(MappingContext)) == false)
        {
            return;
        }

        APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
        if (ensure(IsValid(PlayerController)) == false)
        {
            return;
        }

        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

        if (ensure(IsValid(Subsystem)) == false)
        {
            return;
        }
        Subsystem->AddMappingContext(MappingContext, Priority);
    }
}
