// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceTrader.h"

// Sets default values
AResourceTrader::AResourceTrader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AResourceTrader::Initialize(const FString& ResourceName, ATown* Origin, ATown* Destination)
{
    if (!Destination)
    {
        UE_LOG(LogTemp, Error, TEXT("Initialize failed: TargetTown is null!"));
        return;
    }
    if (!Destination || !IsValid(Destination))
    {
        UE_LOG(LogTemp, Error, TEXT("Initialize failed: Target is null or invalid."));
        return;
    }
	DestinationTown = Destination;
    OriginTown = Origin;
    ResourceToGet = ResourceName;
}

// Called when the game starts or when spawned
void AResourceTrader::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AResourceTrader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AResourceTrader::TransferResources()
{
    if (isReturning)
    {
        // Deposit resources to OriginTown
        for (FResource& Resource : OriginTown->Resources)
        {
            if (Resource.Name == ResourceToGet)
            {
                Resource.ModifyAmount(10); // Arbitrary transfer amount
                break;
            }
        }
    }
    else
    {
        // Take resources from DestinationTown
        for (FResource& Resource : DestinationTown->Resources)
        {
            if (Resource.Name == ResourceToGet)
            {
                Resource.ModifyAmount(-10); // Arbitrary transfer amount
                isReturning = true;
                break;
            }
        }
    }
}



