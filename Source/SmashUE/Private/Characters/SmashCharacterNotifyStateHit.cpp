// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterNotifyStateHit.h"

#include "Interfaces/SmashCharacterHit.h"

void USmashCharacterNotifyStateHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(MeshComp->GetOwner()->Implements<USmashCharacterHit>())
	{
		ISmashCharacterHit::Execute_StartAttackTrace(MeshComp->GetOwner());
	}
}

void USmashCharacterNotifyStateHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(MeshComp->GetOwner()->Implements<USmashCharacterHit>())
	{
		ISmashCharacterHit::Execute_StopAttackTrace(MeshComp->GetOwner());
	}
}
