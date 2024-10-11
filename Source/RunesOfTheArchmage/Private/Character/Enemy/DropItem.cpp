// Copyright DOGU0908


#include "Character/Enemy/DropItem.h"

TArray<FDropItemInfo> UDropItem::GetDropItems()
{
	TArray<FDropItemInfo> DropItems;

	for (auto& DropItemInfo: DropItemInfos)
	{
		for (int32 i = 0; i < DropItemInfo.MaxSpawnNumber; ++i)
		{
			if (FMath::FRandRange(1.f, 100.f) < DropItemInfo.SpawnChance)
			{
				FDropItemInfo NewDropItem;
				NewDropItem.DropItemClass = DropItemInfo.DropItemClass;
				DropItems.Add(NewDropItem);
			}
		}
	}

	// can take level as a parameter to drop different items by level
	// this project will follow an example of maplestory, where a single type of monster does not have a high level difference,
	// dropping only specific items, not taking player level into account

	return DropItems;
}
