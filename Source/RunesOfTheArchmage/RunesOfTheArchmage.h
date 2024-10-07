// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_INTERACTION 250

// custom collision channel for projectile overlap
// set background objects and enemies to generate overlap events, and overlap projectile collision channel
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Ground ECollisionChannel::ECC_GameTraceChannel2