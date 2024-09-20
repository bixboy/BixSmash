﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SmashCharacterNotifyEndAttack.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifiedSignature); 
UCLASS()
class SMASHUE_API USmashCharacterNotifyEndAttack : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
