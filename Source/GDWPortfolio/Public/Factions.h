// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factions.generated.h"

USTRUCT(BlueprintType)
struct FFaction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FName Name; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    FColor Color;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Faction")
    TArray<class ATown*> OwnedTowns;

    // Key: Faction name, Value: Relationship score (-100 = hostile, 0 = neutral, 100 = friendly)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
    TMap<FName, int32> Relationships;

    FFaction()
    {
        Name = "Unknown";
        Color = FColor::White;
    }

    FFaction(const FName& InName)
        : Name(InName)
    {
    }
};
