/* Class was created by Erik Dubbelboer (Amsterdam).

This class is being used upon his permission which can be proved by this link:
https://gist.github.com/erikdubbelboer/cd674cd29399bc9bddb9c9c8a6bd7886 (see comments)

Class adds new distribution vector parameter which allows uniform distribution.
The reason it's added is bacause engine allows you to have uniform distribution only with float parameters.

To better understand what are these types of parameters google 'particle instance parameters'.

*/

#pragma once

#include "CoreMinimal.h"
#include "Distributions/DistributionVector.h"
#include "DistributionVectorUniformParam.generated.h"


UCLASS(collapsecategories, hidecategories = Object, editinlinenew)
class OCEANPLUGIN_API UDistributionVectorUniformParam : public UDistributionVector
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere)
	FName MaxParameterName;

	UPROPERTY(EditAnywhere)
	FName MinParameterName;

	UPROPERTY(EditAnywhere)
	FVector DefaultMax;

	UPROPERTY(EditAnywhere)
	FVector DefaultMin;

	virtual FVector	GetValue(float F = 0.f, UObject* Data = NULL, int32 LastExtreme = 0, struct FRandomStream* InRandomStream = NULL) const override;
	virtual	void GetRange(FVector& OutMin, FVector& OutMax) const override;

	virtual bool CanBeBaked() const override { return false; }
	virtual bool IsPostLoadThreadSafe() const override { return true; };

public:
	UDistributionVectorUniformParam();

private:
	bool GetParamValue(UObject* Data, FName ParamName, FVector& OutVector) const;
};
