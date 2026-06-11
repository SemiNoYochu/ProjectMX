#pragma once

#include "CoreMinimal.h"

namespace ComponentUtils
{
    template<typename T>
    FName MakeComponentName(AActor* Owner, FName createName);   // Forward Declare MakeComponent<T> Template Function
    
    template<typename T>
    T* CreateComponent(
        AActor* Owner,                          //필수 클래스
        bool isChild = true,                    //하위 계층 인지
        USceneComponent* InParent = nullptr,    //상위 레퍼런스
        FName createName = NAME_None,          //이름
        FName InSocketName = NAME_None)
    {
        static_assert(TIsDerivedFrom<T, USceneComponent>::IsDerived, "T is Not USceneComponent");

        if (!ensureMsgf(Owner, TEXT("CreateComponent Owner Null")))
        {
            return nullptr;
        }

        FName finalName = MakeComponentName<T>(Owner, createName);

        T* NewComp = Owner->CreateDefaultSubobject<T>(finalName);
        if (!ensureMsgf(NewComp, TEXT("CreateComponent CreateDefaultSubobject Failed ")))
        {
            return nullptr;
        }
     
        if (isChild)                    //하위 계층 인지
        {
            if (InParent == nullptr)    //상위 레퍼런스 체크
            {
                InParent = Owner->GetRootComponent();
            }
            
            if (ensureMsgf(InParent, TEXT("GetRootComponent Parent Null")))
            {
                NewComp->SetupAttachment(InParent, InSocketName);
            }
        }
        else 
        {
            Owner->SetRootComponent(NewComp);
        }

        return NewComp;
    } //CreateComponent

    template<typename T>
    T* CreateRootComponent(
        AActor* Owner,
        FName createName = NAME_None)
    {
        T* t = CreateComponent<T>(Owner, false, nullptr, createName);
        return t;
    }

    template<typename T>
    T* CreateComponentWithLocation(
        AActor* Owner,
        FVector vec = FVector::ZeroVector,
        bool isChild = true,
        USceneComponent* InParent = nullptr,
        FName createName = NAME_None)
    {
        T* t = CreateComponent<T>(Owner, isChild, InParent, createName);

        if (ensureMsgf(t, TEXT("GetRootComponent Parent Null")))
        {
            t->SetRelativeLocation(vec);
        }

        return t;
    }

    template<typename T>
    T* CreateActorComponent(
        AActor* Owner,
        FName createName = NAME_None)
    {
        static_assert(TIsDerivedFrom<T, UActorComponent>::IsDerived, "T is Not UActorComponent");

        if (!ensureMsgf(Owner, TEXT("CreateActorComponent Owner Null")))
        {
            return nullptr;
        }

        FName finalName = MakeComponentName<T>(Owner, createName);

        T* NewComp = Owner->CreateDefaultSubobject<T>(finalName);

        if (!ensureMsgf(NewComp, TEXT("CreateActorComponent Failed")))
        {
            return nullptr;
        }

        return NewComp;
    }

    template<typename T>
    FName MakeComponentName(AActor* Owner, FName createName = NAME_None)
    {
        if (!createName.IsNone())
        {
            return createName;
        }

        //자동으로 이름 생성
        return MakeUniqueObjectName(Owner, T::StaticClass(), T::StaticClass()->GetFName());
    }
    
}
