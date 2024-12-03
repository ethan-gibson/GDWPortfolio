// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resource.h"
#include "Factions.h"
#include "Town.generated.h"

class AResourceTrader;

UCLASS()
class GDWPORTFOLIO_API ATown : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<FResource> Resources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trade")
	TSubclassOf<AActor> ResourceTraderClass;
	
	UFUNCTION(BlueprintCallable, Category = "Trade")
	void SpawnTrader(const FString& ResourceName, ATown* DestinationTown);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	float MinAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Town")
	FFaction TownFaction; // The faction this town belongs to

	UFUNCTION(BlueprintCallable, Category = "Town")
	bool CanTradeWith(const ATown* OtherTown) const;

	UFUNCTION(BlueprintCallable, Category = "Town")
	void ChangeFaction(const FName& NewFactionName);

	UFUNCTION(BlueprintCallable)
	void HandleTraderDestroyed(AActor* DestroyedActor);

private:
	ATown* FindTownWithResource(const FString& ResourceName);

	UFUNCTION(BlueprintCallable, Category = "Trade")
	void CheckResources();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TArray<FResource> ConsumedResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TArray<FResource> ProducedResources;

	UPROPERTY()
	TMap<FString, AResourceTrader*> ActiveTraders; //key: resource, Value: price\

	float GetResourcePrice(FResource Resource);

	bool HasActiveTraderForResource(const FString& ResourceName) const;

	// Mark a resource as having an active trader
	void AddActiveTrader(const FString& ResourceName, AResourceTrader* Trader);

	// Remove an active trader when it's done
	void RemoveActiveTrader(const FString& ResourceName);

	FTimerHandle ResourceCheckTimerHandle;

	void OnChangeFactionRelationship();
};
