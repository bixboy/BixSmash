	// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"

#include "Characters/States/SmashCharacterStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/SmashCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/SmashCharacterHit.h"
#include "Kismet/KismetSystemLibrary.h"


ASmashCharacter::ASmashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	
	InitStateMachine();

	FallHorizontalMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	FallGravityScale = GetCharacterMovement()->GravityScale;
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextInToController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputMoveXAxisAndActions(EnhancedInputComponent);
}

#pragma region Orient X

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}	

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

#pragma endregion

#pragma region State Machine

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

#pragma endregion

void ASmashCharacter::SetupMappingContextInToController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

	float ASmashCharacter::GetInputMoveX() const
	{
		return InputMoveX;
	}

	float ASmashCharacter::GetInputMoveZ() const
	{
		return  InputMoveZ;
	}

	void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
	{
		if(InputComponent == nullptr) return;

		if (InputData->InputActionMoveX)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveX,
				ETriggerEvent::Started,
				this,
				&ASmashCharacter::OnInputMoveX
			);

			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveX,
				ETriggerEvent::Completed,
				this,
				&ASmashCharacter::OnInputMoveX
			);
			
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveX,
				ETriggerEvent::Triggered,
				this,
				&ASmashCharacter::OnInputMoveX
			);
		}

		if (InputData->InputActionMoveXFast)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveXFast,
				ETriggerEvent::Triggered,
				this,
				&ASmashCharacter::OnInputMoveXFast
				);
		}

		if (InputData->InputActionMoveZ)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveZ,
				ETriggerEvent::Started,
				this,
				&ASmashCharacter::OnInputMoveZ
			);

			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveZ,
				ETriggerEvent::Completed,
				this,
				&ASmashCharacter::OnInputMoveZ
			);
			
			EnhancedInputComponent->BindAction(
				InputData->InputActionMoveZ,
				ETriggerEvent::Triggered,
				this,
				&ASmashCharacter::OnInputMoveZ
			);
		}

		if (InputData->InputActionJump)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionJump,
				ETriggerEvent::Started,
				this,
				&ASmashCharacter::OnInputJump
			);
		}

		if (InputData->InputActionAttack)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionAttack,
				ETriggerEvent::Started,
				this,
				&ASmashCharacter::OnInputAttack
			);
		}
	}

#pragma region MovementInput

	void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
	{
		InputMoveX = InputActionValue.Get<float>();	
	}

	void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
	{
		InputMoveX = InputActionValue.Get<float>();
		InputMoveXFastEvent.Broadcast(InputMoveX);
	}

	void ASmashCharacter::OnInputMoveZ(const FInputActionValue& InputActionValue)
	{
		InputMoveZ = InputActionValue.Get<float>();
	}

	void ASmashCharacter::Move(float Speed, float Orient)
	{
		GetCharacterMovement()-> MaxWalkSpeed = Speed;
		AddMovementInput(GetActorForwardVector(), Orient);
	}

#pragma endregion

#pragma region Jump Actions
	void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
	{
	if (StateMachine && JumpIndex != MaxJump)
    	{
			JumpIndex ++;
    	    StateMachine->ChangeState(ESmashCharacterStateID::Jump);
    	}
	}

	void ASmashCharacter::Jumping()
	{
		Jump();
	}

	void ASmashCharacter::Fall()
	{
		StopJumping();
	}

	void ASmashCharacter::Landed(const FHitResult& Hit)
	{
		Super::Landed(Hit);
	    GetCharacterMovement()->MaxWalkSpeed = FallHorizontalMoveSpeed;
		GetCharacterMovement()->GravityScale = FallGravityScale;
		JumpIndex = 0;
	}

#pragma endregion

#pragma region Attack

bool ASmashCharacter::GetIsAttacking() const
{
	return InAttack;
}

void ASmashCharacter::OnInputAttack(const FInputActionValue& InputActionValue)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Attack);
	InAttack = true;

	HitActors.Empty();
}

void ASmashCharacter::EndAttack()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	InAttack = false;
}

void ASmashCharacter::TakeDamage_Implementation(FHitResult HitResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Im Hit"));
	//Impact Sound

	LaunchCharacter(FVector(KnockBackStrength, 0.f, fabs(KnockBackStrength)), true, true);
	
	//AnimMontage	
}

void ASmashCharacter::StartAttackTrace_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTraceTimerHandle,
		this,
		&ASmashCharacter::AttackTraceLoop,
		0.001f,
		true);
}

void ASmashCharacter::StopAttackTrace_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTraceTimerHandle);
}

void ASmashCharacter::AttackTraceLoop()
{
	if (CanHit)
	{
		const FVector Start = this->GetMesh()->GetSocketLocation("LeftHand");
		const FVector End = Start + FVector(AttackRange, 0, 0);
       
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult HitResult;
       
		// Sphere Trace
		const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			Start,
			End,
			TraceRadius,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true,
			FLinearColor::Gray,
			FLinearColor::Blue,
			0.5f
		);
		
		// Si quelque chose est touché
		if (bHit && HitResult.GetActor() != nullptr)
		{
			AActor* HitActor = HitResult.GetActor();
           
			if (HitActor->Implements<USmashCharacterHit>() && !HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);
				ISmashCharacterHit::Execute_TakeDamage(HitActor,HitResult);
				return;
			}
		}
	}
}

#pragma endregion
