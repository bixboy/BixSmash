#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"

void ULocalMultiplayerSubsystem::CreatAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	ULocalMultiplayerSettings* Settings = GetMutableDefault<ULocalMultiplayerSettings>();
	for (int32 i = 0; i < Settings->GetNbKeyboardProfiles(); ++i)
	{
		FString Error;
		GetGameInstance()->CreateLocalPlayer(i, Error, true);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyProfileIndex(int KeyboardProfileIndex)
{
	for (int32 PlayerIndex = 0; PlayerIndex < GetGameInstance()->GetNumLocalPlayers(); ++PlayerIndex)
	{
		ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex);
		if (LocalPlayer && LocalPlayer->PlayerController)
		{
			return PlayerIndex;
		}
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	LastAssignedPlayerIndex++;
	return  PlayerIndexFromGamepadProfileIndex[KeyboardProfileIndex];
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	if (PlayerIndex < 0 || PlayerIndex >= GetGameInstance()->GetNumLocalPlayers())
	{
		return;
	}

	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings)
	{
		return;
	}

	if (KeyboardProfileIndex < 0 || KeyboardProfileIndex >= Settings->KeyboardProfilesData.Num())
	{
		return;
	}

	const UInputMappingContext* IMC = Settings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
	if (!IMC)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex);
	if (!LocalPlayer || !LocalPlayer->PlayerController)
	{
		return;
	}

	LocalPlayer->GetSubsystem<ULocalMultiplayerSettings>();
	
}
