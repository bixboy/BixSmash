// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateHit.h"

#include "Characters/SmashCharacter.h"
#include "Characters/States/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateHit::GetStateID()
{
	return ESmashCharacterStateID::Hit;
}

void USmashCharacterStateHit::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);
}

void USmashCharacterStateHit::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateHit::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

