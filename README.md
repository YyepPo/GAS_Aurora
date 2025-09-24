# GAS_Aurora -> WORK IN PROGRESS

This Unreal Engine 5 project is a multiplayer third-person action game built with C++ and Gameplay Ability System (GAS). The primary goal is to learn and master GAS for multiplayer use, focusing on ability management, attribute systems, physical animations, UI materials, character movement and networking.

The player will control Aurora (from Epic Games), utilizing a range of melee attacks, GAS-powered abilities.

Descriptions

Character: Aurora
  - Turn-in-place (90 degrees)
  - Walk start / stop
  - Jumping
  - Foot IK for realistic foot placement
  - Sword attack animation
  - Physical animations for hit reactions (using hit directions)

Gameplay Systems

Core Combat System
Melee Attacks – Basic attacks using a sword that cost stamina and deal damage to enemies.
Abilities (x3) – Aurora will have three unique abilities, each using Gameplay Ability System. These abilities:
  - Consume mana
  - Have individual cooldowns
  - Are activated through animation montages
  - Temporarily disable the other two abilities while one is active
  - Abilities will be replicated and have visual/audio feedback.
    
Hit Reactions – Triggered by physical animations when Aurora takes damage, creating immersive feedback.

Attribute System
Each attribute has its own progress bar with delayed-follow UI to emphasize resource changes:
  - Health – Reduced by incoming damage.
  - Mana – Reduced by ability use.
  - Stamina – Reduced by melee attacks or other physical actions.

UI
 - Most of the UI elements are material-based, enabling smooth transitions, animations, and stylized visual effects.
 - Each attribute (Health, Mana, Stamina) has a progress bar with a "ghost bar" that trails behind to visually indicate resource depletion over time.
 - Ability icons will display cooldowns, input key, and availability status, dynamically updated via GAS tags and material parameters.
 - The UI is designed for responsiveness and clarity in a multiplayer combat environment, with performance in mind.
