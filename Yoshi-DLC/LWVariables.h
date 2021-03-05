#pragma once

void** const MESSAGE_MANAGER = (void**)ASLR(0x00FD4300);

float DEG_TO_RAD = 0.017453292f;

char* GOCColliderString = (char*)ASLR(0x00D6425C);
char* GOCVisual = (char*)ASLR(0x00D617C8);
char* const GOCGravityString = (char*)ASLR(0x00D64314);
char* GOCShadowString = (char*)ASLR(0x00D64434);
char* GOCTransformString = (char*)ASLR(0x00D60B44);
char* const GOCSoundString = (char*)ASLR(0x00D64450);
char* GOCEffectString = (char*)ASLR(0x00D642B4);
char* GOCAnimation = (char*)ASLR(0x00D64E9C);
char* const CVisualGOCString = (char*)ASLR(0x00E01360);
char* const GOCHudString = (char*)ASLR(0x00D64334);
char* const GOCAnimationString = (char*)ASLR(0x00D64E9C);
char* const GOCLauncherString = (char*)ASLR(0x00D6435C);
char* const GOCMovementString = (char*)ASLR(0x00D643E8);
char* const GOCEnemyHsmString = (char*)ASLR(0x00D9C674);

int* GOCTransform = (int*)ASLR(0x00F68ED4);
int* GOCAnimationScript = (int*)ASLR(0x00FD7518);
int* GOCVisualContainer = (int*)ASLR(0x00FD4254);
int* GOCVisualModel = (int*)ASLR(0x00FD4238);
int* GOCAnimationSimple = (int*)ASLR(0x00FD7534);
int* GOCGravity = (int*)ASLR(0x00FD76D8);
int* GOCCollider = (int*)ASLR(0x00FD75A4);
int* GOCSound = (int*)ASLR(0x00FD7908);
int* GOCShadowSimple = (int*)ASLR(0x00FD78EC);
int* GOCEffect = (int*)ASLR(0x00FD7630);
int* GOCMotorRotate = (int*)ASLR(0x00FD77D4);
int* GOCHud = (int*)ASLR(0x00FD7710);
int* GOCLauncher = (int*)ASLR(0x00FD7764);
int* GOCMovementComplex = (int*)ASLR(0x00FD7860);
int* GOCEnemyHSM = (int*)ASLR(0x00FE75B4);