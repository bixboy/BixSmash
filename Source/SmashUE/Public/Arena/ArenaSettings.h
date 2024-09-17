// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacter.h"
#include "Engine/DeveloperSettings.h"
#include "ArenaSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Smash ArenaSettings"))
class SMASHUE_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP0;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP1;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP2;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ASmashCharacter> SmashCharacterClassP3;
};
