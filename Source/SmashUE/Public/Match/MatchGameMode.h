﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;
class AArenaPlayerStart;
class ASmashCharacter;

/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:
		virtual void BeginPlay() override;

	protected:
		UPROPERTY()
		TArray<ASmashCharacter*> CharactersInsideArena;
	
	private:
		USmashCharacterInputData* LoadInputDataFromConfig();

		UInputMappingContext* LoadInputMappingContextFromConfig();
	
		void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors);

		void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);

		void CreateAndInitPlayers() const;
	
		TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;
};
