// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Resource.generated.h"


USTRUCT(BlueprintType)
struct FResource {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ProduceComsumeAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Produce;

	float GetValue() const {
		float logAmount = FMath::LogX(2.0f, FMath::Max(1.0f, (float)Amount));
		float price = BaseValue / (1.0f + logAmount * 0.1f);
		price = FMath::Clamp(BaseValue, MinValue, MaxValue);//prevent potential extremes
		return price;
	}

	void ModifyAmount(float Delta)
	{
		Amount += Delta;
		if (Amount < 0)
		{
			Amount = 0; // Ensure no negative resources
		}
	}
};