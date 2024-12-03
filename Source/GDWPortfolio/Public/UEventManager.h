// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Factions.h"
#include "UEventManager.generated.h"



UCLASS(Blueprintable)
class GDWPORTFOLIO_API UUEventManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Delegate for broadcasting faction-related events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
        FFactionEventDelegate,
        const FName&, FactionName1,
        const FName&, FactionName2,
        int32, RelationshipChange
    );

    FFactionEventDelegate OnFactionRelationshipChanged;

    void TriggerFactionRelationshipEvent(const FName& FactionName1, const FName& FactionName2, int32 RelationshipChange);
};
