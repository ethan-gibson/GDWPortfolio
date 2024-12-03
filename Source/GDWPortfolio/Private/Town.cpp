// Fill out your copyright notice in the Description page of Project Settings.


#include "Town.h"
#include "Engine/World.h"
#include "ResourceTrader.h"
#include "Kismet/GameplayStatics.h"
#include "UFactionManager.h"
#include <UFactionManager.h>

// Sets default values
ATown::ATown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATown::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(ResourceCheckTimerHandle, this, &ATown::CheckResources, 1.0f, true);

	ChangeFaction(TownFaction.Name);

}

// Called every frame
void ATown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATown::CheckResources()
{
	// Consume resources
	for (FResource& Resource : Resources)
	{
		if (Resource.Produce) {
				Resource.ModifyAmount(Resource.ProduceComsumeAmount); // Increase by some value
			}
		else {
			Resource.ModifyAmount(-Resource.ProduceComsumeAmount); // Decrease by some value
		}
		
		if (Resource.Amount <= MinAmount)
		{
			ATown* DestinationTown = FindTownWithResource(Resource.Name);
						if (DestinationTown)
			{
				SpawnTrader(Resource.Name, DestinationTown);
			}
		}
	}


}

float ATown::GetResourcePrice(FResource Resource)
{
	return Resource.GetValue();
}

bool ATown::HasActiveTraderForResource(const FString& ResourceName) const
{
	return ActiveTraders.Contains(ResourceName);
}

void ATown::AddActiveTrader(const FString& ResourceName, AResourceTrader* Trader)
{
	UE_LOG(LogTemp, Log, TEXT("Added to list"));
	ActiveTraders.Add(ResourceName, Trader);
	OnChangeFactionRelationship();
}

void ATown::RemoveActiveTrader(const FString& ResourceName)
{
	ActiveTraders.Remove(ResourceName);
}



void ATown::SpawnTrader(const FString& ResourceName, ATown* DestinationTown)
{
	if (HasActiveTraderForResource(ResourceName)) {
		UE_LOG(LogTemp, Log, TEXT("Already Created Trader"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Destination: %s"), *DestinationTown->GetName());
	//if we are low, we spawn a trader to go get the item

		FActorSpawnParameters SpawnParams;
		AActor* TraderActor = GetWorld()->SpawnActor<AActor>(ResourceTraderClass, GetActorLocation() + FVector(100.0f,100.0,0), FRotator::ZeroRotator, SpawnParams);
		if (TraderActor) {
			UE_LOG(LogTemp, Log, TEXT("Created Trader"));
			AResourceTrader* Trader = Cast<AResourceTrader>(TraderActor);
			if (!Trader) {
				UE_LOG(LogTemp, Log, TEXT("cast failed"));
				return;
			}
			Trader->Initialize(ResourceName, this, DestinationTown);
			Trader->Move();
			AddActiveTrader(ResourceName, Trader);
			Trader->OnDestroyed.AddDynamic(this, &ATown::HandleTraderDestroyed);
		}
	
}

ATown* ATown::FindTownWithResource(const FString& ResourceName)
{
	float DistWeight = 0.7f;
	float PriceWeight = 0.3f;
	float BestScore = FLT_MAX;
	ATown* BestTown = nullptr;
	TArray<AActor*> Towns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATown::StaticClass(), Towns);

	for (AActor* Actor : Towns)
	{
		ATown* OtherTown = Cast<ATown>(Actor);
		if (OtherTown && OtherTown != this)
		{
			for (const FResource& Resource : OtherTown->Resources)
			{
				if (Resource.Name == ResourceName && Resource.Amount > 10.0f) // Arbitrary surplus threshold
				{
					float Distance = FVector::Dist(this->GetActorLocation(), OtherTown->GetActorLocation());
					float Price = OtherTown->GetResourcePrice(Resource);

					float score = (Distance * DistWeight) + (Price * PriceWeight);
					if (score < BestScore) {
						BestScore = score;
						BestTown = OtherTown;
					}
				}
			}
		}
	}
	if (BestTown) {
		return BestTown;
	}
	UE_LOG(LogTemp,Warning,TEXT("No Town Found"));
	return nullptr;
}
bool ATown::CanTradeWith(const ATown* OtherTown) const
{
	if (OtherTown) {
		const int32* RelationshipScore = TownFaction.Relationships.Find(OtherTown->TownFaction.Name);
		if (RelationshipScore && *RelationshipScore >= 0) // Neutral or friendly
		{
			return true;
		}
		return false; // Hostile relationship
	}
	return false; // Hostile relationship
}
void ATown::ChangeFaction(const FName& NewFactionName)
{
	TownFaction = NewFactionName;

	// Find the FactionManager and update the town's faction
	if (UWorld* World = GetWorld())
	{
		if (UUFactionManager* FactionManager = GetWorld()->GetSubsystem<UUFactionManager>())
		{
			FactionManager->AssignTownToFaction(this, NewFactionName);
		}
	}
}
void ATown::HandleTraderDestroyed(AActor* DestroyedActor)
{
	AResourceTrader* Trader = Cast<AResourceTrader>(DestroyedActor);
	if (!Trader)
		return;

	// Find the resource name associated with this trader
	FString ResourceName;
	for (const auto& Entry : ActiveTraders)
	{
		if (Entry.Value == Trader)
		{
			ResourceName = Entry.Key;
			break;
		}
	}

	if (!ResourceName.IsEmpty())
	{
		RemoveActiveTrader(ResourceName);
		UE_LOG(LogTemp, Log, TEXT("Trader for resource %s destroyed"), *ResourceName);
	}
}
void ATown::OnChangeFactionRelationship()
{
	// Get the EventManager subsystem
	UUEventManager* EventManager = GetWorld()->GetSubsystem<UUEventManager>();
	if (EventManager)
	{
		// Trigger the faction event
		UE_LOG(LogTemp, Log, TEXT("Caller"));
		EventManager->TriggerFactionRelationshipEvent("FactionA", "FactionB", -100);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get EventManager subsystem!"));
	}
}