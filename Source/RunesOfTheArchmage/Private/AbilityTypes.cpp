
#include "AbilityTypes.h"


UScriptStruct* FGameplayEffectContextBase::GetScriptStruct() const
{
	return StaticStruct();
}

bool FGameplayEffectContextBase::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// copied from GameplayEffectContext
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		// add more data to serialize
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 7;
		}
	}

	Ar.SerializeBits(&RepBits, 8);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	// add more data to serialize
	if (RepBits & (1 << 7))
	{
		Ar << bIsCriticalHit;
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}

bool FGameplayEffectContextBase::IsCriticalHit() const
{
	return bIsCriticalHit;
}

void FGameplayEffectContextBase::SetIsCriticalHit(const bool bInIsCriticalHit)
{
	bIsCriticalHit = bInIsCriticalHit;
}

FGameplayEffectContextBase* FGameplayEffectContextBase::Duplicate() const
{
	// copied from GameplayEffectContext
	FGameplayEffectContextBase* NewContext = new FGameplayEffectContextBase();
	*NewContext = *this;
	if (GetHitResult())
	{
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}
