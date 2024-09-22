// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SmashCharacterHit.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USmashCharacterHit : public UInterface
{
	GENERATED_BODY()
};


class SMASHUE_API ISmashCharacterHit
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(FHitResult HitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartAttackTrace();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopAttackTrace();
};
