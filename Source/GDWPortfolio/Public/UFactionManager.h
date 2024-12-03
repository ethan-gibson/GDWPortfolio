// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UEventManager.h"
#include "Factions.h"
#include "UFactionManager.generated.h"

/**
 * 
 */
UCLASS()
class GDWPORTFOLIO_API UUFactionManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	TArray<FFaction> Factions;

    void HandleFactionRelationshipChange(const FName& FactionName1, const FName& FactionName2, int32 RelationshipChange);

    void UpdateRelationshipsOnTownAssignment(const FName& FactionName);

    FFaction* GetFactionByName(const FName& FactionName);

    void ModifyFactionRelationship(FFaction& Faction, const FName& OtherFactionName, int32 Amount);
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    void InitializeEventManager(UUEventManager* EventManager);

    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    void AddFaction(const FName& FactionName);

    UFUNCTION(BlueprintCallable, Category = "Faction Management")
    void AssignTownToFaction(class ATown* Town, const FName& FactionName);


};
