// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleHandler.h"
#include "Particles/ParticleSystemComponent.h"
#include "OceanManager.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"


UParticleHandler::UParticleHandler(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
	
	ID = 0;
	frontRearGradient = 1.f;
	spawnRateMultiplier_Inst = 1.f;
	initialVelocityMultiplier_Inst = 1.f;

	spawnRate_Inst = 0.f;
	initialVelocity_Inst = FVector(0.f, 0.f, 0.f);
	initialLocationMax_Inst = FVector(0.f, 0.f, 0.f);

	PSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PSC"));
	
	// change this if you want to specify different particle system otherwise no particle system will be loaded
	static ConstructorHelpers::FObjectFinder<UParticleSystem> particle(TEXT("ParticleSystem'/Game/Assets/Particles/Splashes/P_Splash.P_Splash'"));
	if (particle.Succeeded())
	{
		PSC->SetTemplate(particle.Object);
	}

	PSC->bAutoActivate = true;
	PSC->SetWorldLocationAndRotation(GetComponentLocation(), FRotator::ZeroRotator);
	PSC->SetupAttachment(this);
	//PSC->bEditableWhenInherited = true;
	//PSC->ActivateSystem(true);
}


void UParticleHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	AActor* owner = Cast<AActor>(GetOwner());
	UWorld* world = GetWorld();
	if (!oceanManager)
	{
		for (TActorIterator<AOceanManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
		oceanManager = Cast<AOceanManager>(*ActorItr);
		break;
		}
	}

	if (owner && oceanManager)
	{
		float waveHeight = oceanManager->GetWaveHeight(PSC->GetComponentLocation(), world);
		float PSCHeight = GetComponentLocation().Z;
		PSC->SetVectorParameter("InitialLocation", FVector(0.f, 0.f, waveHeight - PSCHeight - 10.f));

		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Height is %f"), PSCHeight - waveHeight));

		//spawnRate_Inst = FMath::Clamp(PSCHeight - waveHeight, 0.f, 5000.f);
		//PSC->SetFloatParameter("SpawnRate", spawnRate_Inst);
		PSC->SetVectorParameter("InitialVelocity", initialVelocity_Inst);
		PSC->SetFloatParameter("SpawnRate", spawnRate_Inst * owner->GetVelocity().Size());



		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Multiplier is %f, SpawnRate is %f"), frontRearGradient, spawnRate_Inst));

	}
}
