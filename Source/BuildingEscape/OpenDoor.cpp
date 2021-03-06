// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "DoorLight.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (PedastalActors.Num() < 1) {
		UE_LOG(LogTemp, Error, TEXT("Empty PedastalActors on %s"), *GetOwner()->GetName());
	} else {
		for (const auto& PedastalActor : PedastalActors) {
			if (!PedastalActor) {
				UE_LOG(LogTemp, Error, TEXT("null Pedastal in PedastalActors on %s"), *GetOwner()->GetName());
			} else {
				UPedastal* Pedastal = PedastalActor->FindComponentByClass<UPedastal>();
				if (!Pedastal) {
					UE_LOG(LogTemp, Error, TEXT("Missing Pedastal component in PedastalActor %s on %s"), *PedastalActor->GetName(), *GetOwner()->GetName());
				}
				else {
					Pedastals.Add(Pedastal);
				}
			}
		}
	}

	if (DoorLightActors.Num() < 1) {
		UE_LOG(LogTemp, Warning, TEXT("Empty DoorLightActors on %s"), *GetOwner()->GetName());
	}
	else {
		for (const auto& DoorLightActor : DoorLightActors) {
			if (!DoorLightActor) {
				UE_LOG(LogTemp, Error, TEXT("null DoorLight in DoorLightActors on %s"), *GetOwner()->GetName());
			} else {
				UDoorLight* DoorLight = DoorLightActor->FindComponentByClass<UDoorLight>();
				if (!DoorLight) {
					UE_LOG(LogTemp, Error, TEXT("Missing DoorLight component in DoorLightActor %s on %s"), *DoorLightActor->GetName(), *GetOwner()->GetName());
				}
				else {
					DoorLights.Add(DoorLight);
				}
			}
		}
	}

	if (DoorLightActors.Num() != PedastalActors.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Mismatched number of DoorlightActors and PedastalActors on %s"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetLightColors();
	if (AreAllPedastalsCorrectlyMounted()) {
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
	}
}

bool UOpenDoor::AreAllPedastalsCorrectlyMounted()
{
	bool AllCorrect = true;

	for (const auto& Pedastal : Pedastals) {
		if (!Pedastal->IsCorrectStatueMounted()) {
			AllCorrect = false;
		}
	}

	return AllCorrect;
}

void UOpenDoor::SetLightColors()
{
	int Correct = 0;
	int Useful = 0;
	int StatuesMounted = 0;

	// TODO Move to member
	TArray<UStatue*> UsefulStatues;
	for (const auto& Pedastal : Pedastals) {
		UsefulStatues.Add(Pedastal->GetCorrectStatue());
	}

	for (const auto& Pedastal : Pedastals) {
		if (Pedastal->IsAnyStatueMounted()) {
			StatuesMounted++;
		}
		if (Pedastal->IsCorrectStatueMounted()) {
			Correct++;
		} else if (Pedastal->IsUsefulStatueMounted(UsefulStatues)) {
			Useful++;
		}
	}

	if (StatuesMounted == UsefulStatues.Num()) {
		if (LastMountingAttempt.Correct != Correct || LastMountingAttempt.Useful != Useful) {
			if (Correct == UsefulStatues.Num()) {
				OnRight.Broadcast();
			} else {
				OnWrong.Broadcast();
			}
		}
		LastMountingAttempt.Correct = Correct;
		LastMountingAttempt.Useful = Useful;
	}

	/*
	if (LastStatuesMounted != StatuesMounted && LastStatuesMounted != -1) {
		if (Correct == UsefulStatues.Num()) {
			OnRight.Broadcast();
		} else {
			OnWrong.Broadcast();
		}
	}
	LastStatuesMounted = StatuesMounted;
	*/

	for (const auto& DoorLight : DoorLights) {
		if (StatuesMounted < UsefulStatues.Num()) {
			DoorLight->SetColor(UDoorLight::Off);
		} else if (Correct > 0) {
			Correct--;
			DoorLight->SetColor(UDoorLight::Correct);
		} else if (Useful > 0) {
			Useful--;
			DoorLight->SetColor(UDoorLight::Useful);
		}
		else {
			Useful--;
			DoorLight->SetColor(UDoorLight::Wrong);
		}
	}
}