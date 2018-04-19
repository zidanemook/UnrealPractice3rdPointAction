// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE02_API APickup : public AInteractable
{
	GENERATED_BODY()

public:
	APickup();
	
	void BeginPlay() override;

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
		void Use();
	virtual void Use_Implementation();

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		UTexture2D* m_PickupThumbnail;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		FString m_ItemName;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		int32 m_iValue;

	void OnPickedUp();
};
