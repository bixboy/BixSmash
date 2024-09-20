// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterNotifyEndAttack.h"

#include "Characters/SmashCharacter.h"

void USmashCharacterNotifyEndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ASmashCharacter* SmashCharacter = Cast<ASmashCharacter>(MeshComp->GetOwner()))
	{
		SmashCharacter->EndAttack();
	}
}
