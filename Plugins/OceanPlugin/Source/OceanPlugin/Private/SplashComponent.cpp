#include "SplashComponent.h"
#include "ParticleHandler.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Math/Rotator.h"
#include "Math/UnrealMathUtility.h"



USplashComponent::USplashComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TickGroup;
	bWantsOnUpdateTransform = true;
	bAutoActivate = true;
	bAreSplashPointesGenerated = true;
	bUseComplexCollision = true;
	bEditableWhenInherited = true;


	length = 1000.f;
	width = 300.f;
	offsetZ = 0.f;
	density = 10;

	intensityGlobal = 0.f;
	spawnRateGlobal = 0.f;
	initialVelocityGlobal = 3.f;

	//particleSysComp = CreateDefaultSubobject<USplashPSC>(TEXT("SPLASH"));
	//particleSysComp->bAutoManageAttachment = true;
}

// Runs when component is registered
void USplashComponent::OnRegister()
{
	Super::OnRegister();

		drawDebugHelpers();

}

// Runs whenever 'Splash' component was moved (before BeginPlay)
void USplashComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

		drawDebugHelpers();
}

// Runs whenever user has changed any value in the 'Splash' component (before BeginPlay)
// Note: Doesn't work in PIE for some reason
void USplashComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	initializeParticleHandlers();
}

void USplashComponent::initializeParticleHandlers()
{
	AActor* owner = Cast<AActor>(GetOwner());
	if (owner && particleHandlers.IsValidIndex(0))
	{
		for (int32 i = 0; i < particleHandlers.Num(); i++)
		{

			// Initial Velocity
			FVector ownerSideVector = i % 2 == 0 ? owner->GetActorRightVector() : -(owner->GetActorRightVector());
			int32 rotateAlongActorX = i % 2 == 0 ? 45 : -45;
			int32 rotateAlongActorZ = i % 2 == 0 ? -45 : 45;

			FVector rotatedX = (ownerSideVector * 100.f).RotateAngleAxis(rotateAlongActorX, owner->GetActorForwardVector());
			FVector rotatedXZ = rotatedX.RotateAngleAxis(rotateAlongActorZ, owner->GetActorUpVector());

			particleHandlers[i]->initialVelocity_Inst = rotatedXZ * initialVelocityGlobal;

			// Spawn Rate
			particleHandlers[i]->frontRearGradient = 1 - ((float)i / (float)particleHandlers.Num());
			particleHandlers[i]->spawnRate_Inst = particleHandlers[i]->frontRearGradient * spawnRateGlobal;
		}
	}
}

// Draws the grid to visulize traces
void USplashComponent::drawDebugHelpers()
{
	parentActor = Cast<AActor>(GetOwner());
	if (parentActor)
	{
		if (bAreSplashPointesGenerated
			&& particleHandlers.IsValidIndex(0)
			&& particleHandlers[0] != nullptr)
		{
			if (GetWorld())
			{
				FlushPersistentDebugLines(GetWorld());
				for (int32 i = 0; i < particleHandlers.Num(); i++)
				{
					//DrawDebugSphere(GetWorld(), particleHandlers[i]->GetComponentLocation(), 30.f, 3, FColor::Green, true, -1.f, 0, 3.5f);
					//particleHandlers[i]->intensityMultiplier = 1 - (i / particleHandlers.Num());
				}
			}
		}
		else
		{
			particleHandlers.Empty();
			if (GetWorld()) FlushPersistentDebugLines(GetWorld());

			FVector actorLocation = parentActor->GetActorLocation();
			FVector compLocation = GetComponentLocation();
			FVector actorForwardVector = parentActor->GetActorForwardVector() * length;
			FVector actorRightVector = parentActor->GetActorRightVector() * width;
			FVector actorUpVector = parentActor->GetActorUpVector() * offsetZ;

			FVector frontRight = actorForwardVector + actorRightVector + compLocation + actorUpVector;
			FVector rearRight = actorRightVector - actorForwardVector + compLocation + actorUpVector;
			FVector frontLeft = actorForwardVector - actorRightVector + compLocation + actorUpVector;
			FVector rearLeft = -actorForwardVector - actorRightVector + compLocation + actorUpVector;

			DrawDebugLine(GetWorld(), frontRight, rearRight, FColor::Green, true, -1.f, 0, 3.5f);
			DrawDebugLine(GetWorld(), frontLeft, rearLeft, FColor::Blue, true, -1.f, 0, 3.5f);

			if (linesToTraceFromRight.IsValidIndex(0) || linesToTraceFromLeft.IsValidIndex(0) || traceStart.IsValidIndex(0))
			{
				linesToTraceFromRight.Empty();
				linesToTraceFromLeft.Empty();
				traceStart.Empty();
				traceEnd.Empty();
			}

			linesToTraceFromRight = distributeVectorsAcrossLine(frontRight, rearRight, density);
			linesToTraceFromLeft = distributeVectorsAcrossLine(frontLeft, rearLeft, density);

			if (GetWorld()
				&& linesToTraceFromRight.IsValidIndex(0)
				&& linesToTraceFromLeft.IsValidIndex(0))
			{
				for (int32 i = 0; i < linesToTraceFromRight.Num(); i++)
				{
					DrawDebugLine(GetWorld(), linesToTraceFromRight[i], linesToTraceFromRight[i] - actorRightVector, FColor::Green, true, -1.f, 0, 3.5f);
				}
				for (int32 i = 0; i < linesToTraceFromLeft.Num(); i++)
				{
					DrawDebugLine(GetWorld(), linesToTraceFromLeft[i], linesToTraceFromLeft[i] + actorRightVector, FColor::Green, true, -1.f, 0, 3.5f);
				}
				for (int32 i = 0; i < linesToTraceFromRight.Num(); i++)
				{
					traceStart.Emplace(linesToTraceFromRight[i]);
					traceStart.Emplace(linesToTraceFromLeft[i]);
					traceEnd.Emplace(linesToTraceFromRight[i] - actorRightVector);
					traceEnd.Emplace(linesToTraceFromLeft[i] + actorRightVector);
				}
			}
		}
	}
}

TArray<FVector> USplashComponent::distributeVectorsAcrossLine(FVector start, FVector end, int32 dens) const
{
	TArray<FVector> vectors;

	// safety check
	if (dens <= 0)
	{
		return vectors;
	}
	else
	{
		dens = FMath::Clamp(dens, 0, 500);
		FVector nextVector;

		for (int32 i = 0; i < dens + 1; i++)
		{
			nextVector.X = FMath::Lerp(start.X, end.X, (float)i / (float)dens);
			nextVector.Y = FMath::Lerp(start.Y, end.Y, (float)i / (float)dens);
			nextVector.Z = FMath::Lerp(start.Z, end.Z, (float)i / (float)dens);
			vectors.Add(nextVector);
		}
		return vectors;
	}
}

// Create components across the boat hull in the locations where the traces hit the complex/simple collision
void USplashComponent::createParticleHandlers()
{	
	if (GetWorld() && traceStart.IsValidIndex(0) && traceEnd.IsValidIndex(0))
	{
		FlushPersistentDebugLines(GetWorld());
		
		if (particleHandlers.IsValidIndex(0))
		{
			particleHandlers.Empty();
		}

		FHitResult outHit;
		FCollisionQueryParams collisionParams;
		collisionParams.bTraceComplex = bUseComplexCollision;

		parentActor = Cast<AActor>(GetOwner());
		if (parentActor)
		{
			USceneComponent* root = Cast<USceneComponent>(parentActor->GetRootComponent());
			if (root)
			{	
				FVector rearLastRightHit;
				FVector rearLastLeftHit;
				float normalOffset = 10.f;

				for (int32 i = 0; i < traceStart.Num(); i++)
				{
					if (GetWorld()->LineTraceSingleByChannel(outHit, traceStart[i], traceEnd[i], ECC_Visibility, collisionParams))
					{
							FVector surfaceNormal = outHit.Normal * normalOffset;
							FVector location = outHit.Location + surfaceNormal;
							//FVector scaledNormal = outHit.Normal * 100.f;

							DrawDebugSphere(GetWorld(), location, 30.f, 3, FColor::Green, true, -1.f, 0, 3.5f);

							// Create particle hanlders
							particleHandlers.Add(NewObject<UParticleHandler>(this));
							particleHandlers.Top()->CreationMethod = EComponentCreationMethod::Native;
							particleHandlers.Top()->RegisterComponent();
							particleHandlers.Top()->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
							particleHandlers.Top()->SetWorldLocationAndRotation(location, FRotator::ZeroRotator);

							// Initial Velocity
							//int32 rotateAlongActorX = particleHandlers.Num() % 2 == 0 ? -45 : 45;
							//DrawDebugLine(GetWorld(),
							//	location,
							//	location + scaledNormal.RotateAngleAxis(rotateAlongActorX, parentActor->GetActorForwardVector()),
							//	FColor::Red, true, 0, 3.0f);
							//particleHandlers.Top()->initialVelocity_Inst = scaledNormal.RotateAngleAxis(rotateAlongActorX, parentActor->GetActorForwardVector());

							//DrawDebugLine(GetWorld(), 
							//particleHandlers.Top()->ID = particleHandlers.Num() - 1;
					}
				}
				bAreSplashPointesGenerated = true;
				initializeParticleHandlers();

				// deactivate last two components but save their location for setInitialLocation function
				//rearLastLeftHit = particleHandlers.Top()->GetComponentLocation();
				//rearLastRightHit = particleHandlers[particleHandlers.Num() - 1]->GetComponentLocation();
				//particleHandlers.Top()->Deactivate();
			//	particleHandlers[particleHandlers.Num() - 1]->Deactivate();

				// Initial Location
				//setInitialLocation(rearLastRightHit, rearLastLeftHit);

			}
		}
	}
}

//void USplashComponent::setInitialLocation(FVector rearLastRightHit, FVector rearLastLeftHit)
//{
//	if (particleHandlers.IsValidIndex(0))
//	{
//		FVector current;
//		FVector next;
//
//		// loop iterates though the right side, then though the left
//		for (int32 i = 0; i < particleHandlers.Num(); i += 2)
//		{
//			current = particleHandlers[i]->GetComponentLocation();
//
//			if (particleHandlers.IsValidIndex(i + 2))
//			{
//				next = particleHandlers[i + 2]->GetComponentLocation();
//				//DrawDebugLine(GetWorld(), current, next, FColor::Red, true, 0, 3.0f);
//				particleHandlers[i]->initialLocationMax_Inst = next - current;
//			}
//			else
//			{
//				if (i % 2 == 0)
//				{
//					i = -1; // start over on the left side
//				}
//			}
//		}
//	}
//}

void USplashComponent::removeParticleHandlers()
{
	
	if (particleHandlers.IsValidIndex(0))
	{
		for (int32 i = 0; i < particleHandlers.Num(); i++)
		{
		particleHandlers[i]->DestroyComponent();
		}
	}

	particleHandlers.Empty();
	bAreSplashPointesGenerated = false;
	drawDebugHelpers();
}

void USplashComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{

}
