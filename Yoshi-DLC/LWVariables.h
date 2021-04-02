#pragma once

void** const MESSAGE_MANAGER = (void**)ASLR(0x00FD4300);

float DEG_TO_RAD = 0.017453292f;

inline static char* const GOCColliderString = (char*)ASLR(0x00D6425C);
inline static char* const GOCVisual = (char*)ASLR(0x00D617C8);
inline static char* const GOCGravityString = (char*)ASLR(0x00D64314);
inline static char* const GOCShadowString = (char*)ASLR(0x00D64434);
inline static char* const GOCTransformString = (char*)ASLR(0x00D60B44);
inline static char* const GOCSoundString = (char*)ASLR(0x00D64450);
inline static char* const GOCEffectString = (char*)ASLR(0x00D642B4);
inline static char* const GOCAnimation = (char*)ASLR(0x00D64E9C);
inline static char* const CVisualGOCString = (char*)ASLR(0x00E01360);
inline static char* const GOCHudString = (char*)ASLR(0x00D64334);
inline static char* const GOCAnimationString = (char*)ASLR(0x00D64E9C);
inline static char* const GOCLauncherString = (char*)ASLR(0x00D6435C);
inline static char* const GOCMovementString = (char*)ASLR(0x00D643E8);
inline static char* const GOCEnemyTargetString = (char*)ASLR(0x00D9C6A0);
inline static char* const GOCEnemyHsmString = (char*)ASLR(0x00D9C674);
inline static char* const GOCMotorString = (char*)ASLR(0x00D66438);

inline static int* GOCTransform = (int*)ASLR(0x00F68ED4);
inline static int* GOCAnimationScript = (int*)ASLR(0x00FD7518);
inline static int* GOCVisualContainer = (int*)ASLR(0x00FD4254);
inline static int* GOCVisualModel = (int*)ASLR(0x00FD4238);
inline static int* GOCAnimationSimple = (int*)ASLR(0x00FD7534);
inline static int* GOCGravity = (int*)ASLR(0x00FD76D8);
inline static int* GOCCollider = (int*)ASLR(0x00FD75A4);
inline static int* GOCSound = (int*)ASLR(0x00FD7908);
inline static int* GOCShadow = (int*)ASLR(0x00FD78D0);
inline static int* GOCShadowSimple = (int*)ASLR(0x00FD78EC);
inline static int* EnemyTargetGOC = (int*)ASLR(0x00FE7608);
inline static int* GOCEffect = (int*)ASLR(0x00FD7630);
inline static int* GOCMotorRotate = (int*)ASLR(0x00FD77D4);
inline static int* GOCHud = (int*)ASLR(0x00FD7710);
inline static int* GOCLauncher = (int*)ASLR(0x00FD7764);
inline static int* GOCMovementComplex = (int*)ASLR(0x00FD7860);
inline static int* GOCEnemyHSM = (int*)ASLR(0x00FE75B4);
inline static char* CStateGOC = (char*)ASLR(0x00DF77D8);