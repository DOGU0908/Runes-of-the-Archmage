# Runes of the Archmage

This project implements the common features of an RPG, with a focus on character status and skill management.

## Features

1. Character Level
    - Each character (both player and enemy) in the game has its own level.
    - Experience points (EXP) for the player character are managed using a temporary attribute through Unreal's Gameplay Ability System (GAS).
    - Players can level up to upgrade their base attributes and skills, earning attribute points and spell points as they progress.
    - Enemies grant EXP to the player based on their level when defeated.

2. Character Stats
   - Base attributes are used to calculate secondary attributes, creating a deeper and more dynamic battle experience.
   - All attributes are handled using Unreal's GAS, with gameplay effects applied to modify them.

3. Character Skills
   - Players can unlock and upgrade spells by spending spell points.
   - Each element offers three types of spells: a passive spell, a projectile-based spell, and an area-of-effect spell.
   - There are 4 total damage types and 3 for player spells, which are used to calculate the total damage, taking into account the target's damage resistance.
   - Each damage type can inflict a different debuff on the target. Debuff details, such as damage type, duration, and chance, are managed through gameplay effect contexts during damage calculation.

## Demo Video

Check out my demo video to see the features in action: [Watch on Youtube](https://youtu.be/c8KAMBCGIto).

- This game is not for release. EXP and damage values are optimized to record demo video.

## Contact

For any inquiries or issues, please contact me at mryesung1@gmail.com
