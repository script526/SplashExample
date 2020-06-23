// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "ParticleHandler.generated.h"

/**
 * 
 */
UCLASS()
class OCEANPLUGIN_API UParticleHandler : public USceneComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
	class UParticleSystemComponent* PSC;

	/* How many particles will be emitted */
	UPROPERTY()
	float spawnRate_Inst;

	/* Describes the direction of the particles in local space and also their velocity (by lenght of this vector) */
	UPROPERTY()
	FVector initialVelocity_Inst;

	UPROPERTY()
	FVector initialLocationMax_Inst;

	UPROPERTY()
	float spawnRateMultiplier_Inst;

	UPROPERTY()
	float initialVelocityMultiplier_Inst;

	UPROPERTY()
	int32 ID;

	/* More particles in the front part of the boat, less in the rear */
	UPROPERTY()
	float frontRearGradient;

	UPROPERTY()
	class AOceanManager* oceanManager;

private:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
