#include "DistributionVectorUniformParam.h"
#include "Particles/ParticleSystemComponent.h"


UDistributionVectorUniformParam::UDistributionVectorUniformParam(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FVector UDistributionVectorUniformParam::GetValue(float F, UObject* Data, int32 Extreme, struct FRandomStream* InRandomStream) const
{
	FVector LocalMax = DefaultMax;
	FVector LocalMin = DefaultMin;

	FVector ParamMax;
	bool bFoundParam = GetParamValue(Data, MaxParameterName, ParamMax);
	if (bFoundParam)
	{
		LocalMax = ParamMax;
	}

	FVector ParamMin;
	bFoundParam = GetParamValue(Data, MinParameterName, ParamMin);
	if (bFoundParam)
	{
		LocalMin = ParamMin;
	}

	float fX = LocalMin.X + (LocalMax.X - LocalMin.X) * DIST_GET_RANDOM_VALUE(InRandomStream);
	float fY = LocalMin.Y + (LocalMax.Y - LocalMin.Y) * DIST_GET_RANDOM_VALUE(InRandomStream);
	float fZ = LocalMin.Z + (LocalMax.Z - LocalMin.Z) * DIST_GET_RANDOM_VALUE(InRandomStream);

	return FVector(fX, fY, fZ);
}

void UDistributionVectorUniformParam::GetRange(FVector& OutMin, FVector& OutMax) const
{
	OutMin = DefaultMin;
	OutMax = DefaultMax;
}

bool UDistributionVectorUniformParam::GetParamValue(UObject* Data, FName ParamName, FVector& OutVector) const
{
	bool bFoundParam = false;

	UParticleSystemComponent* ParticleComp = Cast<UParticleSystemComponent>(Data);
	if (ParticleComp)
	{
		bFoundParam = ParticleComp->GetAnyVectorParameter(ParamName, OutVector);
	}
	return bFoundParam;
}