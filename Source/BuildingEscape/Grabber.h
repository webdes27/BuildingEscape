// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Statue.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float Reach = 100.0;
	
	UInputComponent* Input = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	APlayerController* PlayerController = nullptr;

	UPrimitiveComponent* GrabbedComponent = nullptr;
	UStatue* HighlightedStatue = nullptr;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	void FindPhysicsHandleComponent();
	void FindPlayerController();
	const FHitResult GetFirstPhysicsBodyInReach();
	UStatue* GetFirstStatueInReach();
	FRotator GetGrabRotation();
	FVector GetReachLineEnd();
	FVector GetReachLineStart();
	void UpdatePlayerViewPoint();
	FVector GetPlayerViewPointLocation();
	FRotator GetPlayerViewPointRotation();
	void Grab();
	void Release();
	void SetupInputComponent();

	void OnGrabKeyReleased();
};
