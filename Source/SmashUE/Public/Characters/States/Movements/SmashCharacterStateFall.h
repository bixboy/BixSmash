// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Fall")
	float FallHorizontalMoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Fall")
	float FallAirControl = 1.f;

	UPROPERTY(EditAnywhere, Category = "Fall")
	float FallGravityScale = 3.f;

	UPROPERTY(EditAnywhere, Category = "Fall")
	float FallFastGravityScale = 10.f;

};
