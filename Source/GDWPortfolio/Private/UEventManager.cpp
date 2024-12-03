// Fill out your copyright notice in the Description page of Project Settings.


#include "UEventManager.h"

void UUEventManager::TriggerFactionRelationshipEvent(const FName& FactionName1, const FName& FactionName2, int32 RelationshipChange)
{
	UE_LOG(LogTemp, Log, TEXT("input 3"));
	if (OnFactionRelationshipChanged.IsBound())
	{
		OnFactionRelationshipChanged.Broadcast(FactionName1, FactionName2, RelationshipChange);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Delegate not bound!"));
	}
}
