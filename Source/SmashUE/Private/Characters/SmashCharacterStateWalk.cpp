// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateWalk.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterState.h"
#include "Characters/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Super::StateExit(PreviousStateID);

	Character->PlayAnimMontage(AnimMontage);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Red,
	TEXT("StateWalk")
	);

	if (FMath::Abs(Character->GetInputMoveX()) < USmashCharacterState::GetInputThreshold())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->Move(MoveSpeed, Character->GetOrientX());
	}
}


