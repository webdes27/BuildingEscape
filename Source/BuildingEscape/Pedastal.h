// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Statue.h"
#include "Pedastal.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UPedastal : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPedastal();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UStatue* GetCorrectStatue();
	bool IsAnyStatueMounted();
	bool IsUsefulStatueMounted(TArray<UStatue*> UsefulStatues);
	bool IsCorrectStatueMounted();

private:

	UPROPERTY(EditAnywhere)
		AActor* CorrectStatueActor = nullptr;

	UStatue* CorrectStatue = nullptr;

	TArray<UStatue*> GetMountedStatues();
};
