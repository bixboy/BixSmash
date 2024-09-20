// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpWalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpMaxHeight = 280.f;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpDuration = 1.f;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpAirControl = 1.f;
};
