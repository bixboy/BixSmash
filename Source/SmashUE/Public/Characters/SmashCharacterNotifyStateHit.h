// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SmashCharacterNotifyStateHit.generated.h"

class ASmashCharacter;

UCLASS()
class SMASHUE_API USmashCharacterNotifyStateHit : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UPROPERTY()
	ASmashCharacter* SmashCharacter;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
