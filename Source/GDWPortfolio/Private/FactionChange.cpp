// Fill out your copyright notice in the Description page of Project Settings.


#include "FactionChange.h"
#include "UFactionManager.h"
#include "UEventManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Sets default values
AFactionChange::AFactionChange()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFactionChange::BeginPlay()
{
	Super::BeginPlay();
    OnChangeFactionRelationship();
}

// Called every frame
void AFactionChange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFactionChange::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	InputComponent->BindAction("ChangeFactionRelationship", IE_Pressed, this, &AFactionChange::OnChangeFactionRelationship);
}

void AFactionChange::OnChangeFactionRelationship()
{
    UE_LOG(LogTemp, Log, TEXT("Input 1"));
    // Get the EventManager subsystem
    UUEventManager* EventManager = GetWorld()->GetSubsystem<UUEventManager>();
    if (EventManager)
    {
        UE_LOG(LogTemp, Log, TEXT("Input 2"));
        // Trigger the faction event
        EventManager->TriggerFactionRelationshipEvent("FactionA", "FactionB", -100);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get EventManager subsystem!"));
    }
}

