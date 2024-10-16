﻿#include "LocalMultiplayerSettings.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key,
	ELocalMultiplayerInputMappingType InputMappingType) const
{
	const UInputMappingContext* MappingContext = GetIMCFromType(InputMappingType);

	if (MappingContext)
	{
		for (const FEnhancedActionKeyMapping& Mapping : MappingContext->GetMappings())
		{
			if (Mapping.Key == Key)
			{
				return true;
			}
		}
	}

	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame;
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
	default:
		return nullptr;
	}
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int32 Index = 0; Index < KeyboardProfilesData.Num(); ++Index)
	{
		const FLocalMultiplayerProfileData& ProfileData = KeyboardProfilesData[Index];

		const UInputMappingContext* IMC = ProfileData.GetIMCFromType(MappingType);

		if (IMC)
		{
			for (const FEnhancedActionKeyMapping& Mapping : IMC->GetMappings())
			{
				if (Mapping.Key == Key)
				{
					return Index;
				}
			}
		}
	}
	return -1; 
}

