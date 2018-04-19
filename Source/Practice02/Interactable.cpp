// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Engine.h"

// Sets default values
AInteractable::AInteractable()
{
 
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}


void AInteractable::Interact_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Interact base class: Interact() YOU SHOULD NOT BE SEEING THIS");
}

