// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Practice02GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE02_API APractice02GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APractice02GameModeBase();	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<UUserWidget> wHUD;

	// Variable to hold the widget After Creating it.
	UUserWidget* m_HUD;


protected:
	// Called when the game starts or when spawned
	UFUNCTION(BlueprintCallable, Category = Event)
		virtual void BeginPlay() override;
};
