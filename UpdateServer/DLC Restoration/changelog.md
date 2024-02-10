<b><h1><span style="color:red">This update requires the latest version of Hedgehog Engine 1 Mod Loader (HE1ML). Before playing update HedgeModManager (HMM) and HE1ML.</span></h1></b>

## General Changes
- Both DLCs have been re-written from scratch using LWAPI as the backend for the mod to allow easier future developments to the mod.
- DLCs are now looked up the same way as they were on the Wii U version of the game, meaning the DLCs are now shipped in their respective CPKs (these CPKs contain the pure converted data whilst any modifications are done with file redirection outside of them).
- The DLCs now use HE1ML's append pac feature which allows modifications to be shipped as individual files in 'append archives' to reduce files instead of shipping the entire archive file twice.
- The 'yoshiisland' and 'zelda' attributes in the 'actstgmission.lua' file are now taken into consideration when loading into a stage for things such as hud or player model modifications, just like on the Wii U.
- All Downloadable Content can now be toggled on and off from the mod's settings.
- Added the 5 pre-order DLCs to the mod (turned off by default configure to your need in the mod's settings).
- Cameras triggered by moving through a specific area have been turned off in multiplayer to avoid cameras hijacking Player 1's view.

## The Legend of Zelda Zone DLC Changes
- The water splash particle for the lake has been re-converted as it was previously the ripple particle that would follow the splash effect.
- The peelout particle for Link Sonic has been re-converted as it was previously glitching out due to incorrect platform conversion of the file.
- 'Play as Link Sonic' option now affecs the Game Over screen.
- 'Fix Link Sonic Materials' option has been reworked to fix the material issue on Link Sonic's mouth at runtime as opposed to used an extra model file in the archives.
- 'Play as Link Sonic in Multiplayer' option has been reworked and player 2 now reads from 'Linksonic2p.pac' instead of 'Linksonic_red.pac'. This file appends the original 'Linksonic.pac' archive to reduce filesize.
- 'Always Play with Heart System' has been completely overhauled on the backend as the UI for it is now displayed using runtime logic as opposed to duplicating and combining the archives containing the ui files.
- 'Fix Stalbaby Eye Texture' no longer duplicates the archive file for the enemy as it now uses HE1ML's append pac sytem. 'EnemyStalbabyFix.pac' appends 'EnemyStalbaby.pac'.
- 'Fix Rupee Count from Chests' option has been removed from the mod as it is now permanently applied. This option existed for level mod makers only however they can still use the Green Rupee type for the Treasure Chests to get the same rupee amount as it would give on all 3 rupee types without the fix.
- 'Use Unused Goal Animation' option has been to the DLC. This option enables an unused state for the Triforce, which makes it hover about Sonic after the results sequence, instead of Sonic holding it.
- 'Fix Water Splash Particle Placement' option has been added to the DLC. The options lowers the splash particle's height on the lake to better line it up with the water.