// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateAttack.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterState.h"
#include "Characters/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateAttack::GetStateID()
{
	return ESmashCharacterStateID::Attack;
}

void USmashCharacterStateAttack::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);

}

void USmashCharacterStateAttack::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

}

void USmashCharacterStateAttack::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (FMath::Abs(Character->GetInputMoveX()) > USmashCharacterState::GetInputThreshold() && !Character->GetIsAttacking())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
	else if (FMath::Abs(Character->GetInputMoveX()) < USmashCharacterState::GetInputThreshold() && !Character->GetIsAttacking())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else if (Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	else
	{
		Character->Move(MoveSpeedAttack, Character->GetOrientX());
		Character->SetOrientX(Character->GetInputMoveX());
	}
}

