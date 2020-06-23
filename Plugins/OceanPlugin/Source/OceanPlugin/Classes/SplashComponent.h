#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SplashComponent.generated.h"


UCLASS(ClassGroup = Physics, meta = (BlueprintSpawnableComponent))
class OCEANPLUGIN_API USplashComponent : public USceneComponent
{
	GENERATED_UCLASS_BODY()

	/* Where to shoot traces from in order to get the locations where we should spawn particles */
	UPROPERTY()
	TArray<FVector> linesToTraceFromRight;

	UPROPERTY()
	TArray<FVector> linesToTraceFromLeft;

	UPROPERTY()
	TArray<FVector> traceStart;

	UPROPERTY()
	TArray<FVector> traceEnd;

	UPROPERTY()
	TArray<class UParticleHandler*> particleHandlers;

	/* Traces are represented by a grid which visulizes how traces will shoot.
	The following settings allow you to change grid dementions and also specify the dencity of traces (how they are close to each other) */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	float length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	float width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	float offsetZ; // if you need to move the grid up or down

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	int32 density;

	/* Use complex collision for traces for best results (you need to set it in the mesh settings). Otherwise it will use simple collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	bool bUseComplexCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Splash Settings")
	TEnumAsByte<enum ETickingGroup> TickGroup;

	UPROPERTY()
	bool bAreSplashPointesGenerated; // flow control

	UPROPERTY()
	AActor* parentActor; // boat/ship blueprint

	/* Global variables which affect every particle system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	float intensityGlobal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	float spawnRateGlobal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Splash Settings")
	float initialVelocityGlobal;
	
	/* Distibute points evenly across the line in 3d space */
	UFUNCTION()
	TArray<FVector> distributeVectorsAcrossLine(FVector start, FVector end, int32 dens) const;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Splash Settings")
	void createParticleHandlers();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Splash Settings")
	void removeParticleHandlers();

	UFUNCTION()
	void initializeParticleHandlers();

//	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Splash Settings")
//	void setInitialLocation(FVector rearLastRightHit, FVector rearLastLeftHit);


	virtual void OnRegister() override;
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	void drawDebugHelpers();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};