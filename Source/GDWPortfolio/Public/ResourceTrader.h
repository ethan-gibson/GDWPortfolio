// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Town.h>
#include "ResourceTrader.generated.h"


UCLASS()
class GDWPORTFOLIO_API AResourceTrader : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceTrader();

	void Initialize(const FString& ResourceName, ATown* Origin, ATown* Destination);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	ATown* OriginTown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	ATown* DestinationTown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool isReturning;
	UFUNCTION(BlueprintImplementableEvent, Category = "Trader")
	void Move();

private:
	FString ResourceToGet;

	UFUNCTION(BlueprintCallable)
	void TransferResources();
};
