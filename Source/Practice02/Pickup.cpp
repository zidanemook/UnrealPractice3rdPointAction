// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Practice02.h"
#include "UserCharacter.h"



APickup::APickup()
{
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	InteractableMesh->SetSimulatePhysics(true);

	m_ItemName = FString("Enter an item here... ");
	InteractableHelpText = FString("Press E to pick item up");
	m_iValue = 0;
}

void APickup::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up."), *m_ItemName);
}

void APickup::Interact_Implementation()
{
	AUserCharacter* Character = Cast<AUserCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	OnPickedUp();
}

void APickup::Use_Implementation()
{
	GLog->Log("Ue() from base pickup class : YOU SHOULD NOT BE SEEING THIS");
}

void APickup::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
