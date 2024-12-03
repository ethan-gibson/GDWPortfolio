// Fill out your copyright notice in the Description page of Project Settings.

#include "Town.h"
#include "CoreMinimal.h"
#include "UFactionManager.h"

void UUFactionManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    AddFaction("FactionA");
    AddFaction("FactionB");
}
void UUFactionManager::InitializeEventManager(UUEventManager* EventManager)
{
    if (EventManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("EventManager is valid"));
        EventManager->OnFactionRelationshipChanged.AddDynamic(this, &UUFactionManager::HandleFactionRelationshipChange);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EventManager is invalid"));
    }
}

void UUFactionManager::AddFaction(const FName& FactionName)
{
    if (!Factions.ContainsByPredicate([&](const FFaction& Faction) { return Faction.Name == FactionName; }))
    {
        Factions.Add(FFaction{ FactionName });
    }
}

void UUFactionManager::AssignTownToFaction(ATown* Town, const FName& FactionName)
{
    if (!Town) return;

    FFaction* NewFaction = GetFactionByName(FactionName);
    if (!NewFaction) return;

    // Remove town from any existing faction
    for (FFaction& Faction : Factions)
    {
        Faction.OwnedTowns.Remove(Town);
    }

    // Add the town to the new faction
    NewFaction->OwnedTowns.Add(Town);

    // Update relationships
    UpdateRelationshipsOnTownAssignment(FactionName);

    UE_LOG(LogTemp, Log, TEXT("Town %s assigned to faction %s"), *Town->GetName(), *FactionName.ToString());
}

FFaction* UUFactionManager::GetFactionByName(const FName& FactionName)
{
    return Factions.FindByPredicate([&](const FFaction& Faction) { return Faction.Name == FactionName; });
}

void UUFactionManager::ModifyFactionRelationship(FFaction& Faction, const FName& OtherFactionName, int32 Amount)
{
    if (Faction.Relationships.Contains(OtherFactionName))
    {
        Faction.Relationships[OtherFactionName] += Amount;
    }
    else
    {
        Faction.Relationships.Add(OtherFactionName, Amount);
    }
}


void UUFactionManager::HandleFactionRelationshipChange(const FName& FactionName1, const FName& FactionName2, int32 RelationshipChange)
{
    FFaction* Faction1 = GetFactionByName(FactionName1);
    FFaction* Faction2 = GetFactionByName(FactionName2);

    if (Faction1 && Faction2)
    {
        ModifyFactionRelationship(*Faction1, Faction2->Name, RelationshipChange);
        ModifyFactionRelationship(*Faction2, Faction1->Name, RelationshipChange);

        UE_LOG(LogTemp, Log, TEXT("Relationship between %s and %s changed by %d"),
            *FactionName1.ToString(), *FactionName2.ToString(), RelationshipChange);
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("Factions Dont Exist"));
    }
}
void UUFactionManager::UpdateRelationshipsOnTownAssignment(const FName& FactionName)
{
    FFaction* UpdatedFaction = GetFactionByName(FactionName);
    if (!UpdatedFaction) return;

    // Example logic: Adjust relationships based on the number of towns
    for (FFaction& OtherFaction : Factions)
    {
        if (OtherFaction.Name != FactionName)
        {
            int32 NewRelationshipValue = UpdatedFaction->OwnedTowns.Num() - OtherFaction.OwnedTowns.Num();
            UpdatedFaction->Relationships.Add(OtherFaction.Name, NewRelationshipValue);
            OtherFaction.Relationships.Add(FactionName, NewRelationshipValue);

            UE_LOG(LogTemp, Log, TEXT("Relationship between %s and %s updated to %d"),
                *UpdatedFaction->Name.ToString(), *OtherFaction.Name.ToString(), NewRelationshipValue);
        }
    }
}
