#pragma once

#include "framework.h"
#include "LWStructs.h"
#include "LWTypes.h"
#include "LWVariables.h"

// csl
#include "csl/fnd.h"

// app
#include "app/animation/AnimationResContainer.h"

#include "app/GameObject.h"
#include "app/fnd/Message.h"
#include "app/CGOCCollectionImpl.h"
#include "app/CLevelInfo.h"
#include "app/CObjInfo.h"
#include "app/CSetAdapter.h"
#include "app/CSetObjectListener.h"
#include "app/EnemyBlowOffObjectCInfo.h"
#include "app/EnemyBase.h"
#include "app/EnemyInfo.h"
#include "app/GameDocument.h"
#include "app/game/CollisionObjCInfo.h"
#include "app/ObjUtil.h"
#include "app/game/GOCLauncher.h"
#include "app/SetShotInfo.h"

#include "app/fnd/CActor.h"
#include "app/fnd/GameService.h"
#include "app/fnd/GameServiceClass.h"
#include "app/fnd/GOComponent.h"
#include "app/fnd/GOCTransform.h"
#include "app/fnd/GOCVisualContainer.h"
#include "app/fnd/HFrame.h"
#include "app/fnd/MessageManager.h"
#include "app/fnd/ReferencedObject.h"

#include "app/game/CollisionShapeType.h"
#include "app/game/GOCAnimationScript.h"
#include "app/game/GOCAnimationSimple.h"
#include "app/game/GOCCollider.h"
#include "app/game/GOCEffect.h"
#include "app/game/GOCGravity.h"
#include "app/game/GOCHud.h"
#include "app/game/GOCMotorRotate.h"
#include "app/game/GOCShadowSimple.h"
#include "app/game/GOCSound.h"
#include "app/game/GOCVisualModel.h"
#include "app/game/GOCMovement.h"
#include "app/game/MoveStraight.h"
#include "app/game/ShadowShapeCInfo.h"

#include "app/Gimmick/CRingManager.h"

#include "app/enemy/DeadEffectCInfo.h"

#include "app/HUD/SRUtility.h"

#include "app/math/CalculatedTransform.h"

#include "app/Player/CPhysics.h"

#include "app/xgame.h"
#include "app/xgame/MsgCatchEndPlayer.h"
#include "app/xgame/MsgCatchPlayer.h"
#include "app/xgame/MsgDamage.h"
#include "app/xgame/MsgExtendPlayer.h"
#include "app/xgame/MsgGetExternalMovePosition.h"
#include "app/xgame/MsgKick.h"
#include "app/xgame/MsgNotifyObjectEvent.h"
#include "app/xgame/MsgPLGetInputButton.h"
#include "app/xgame/MsgPLVisibleItemEffect.h"
#include "app/xgame/MsgResumeGameTimer.h"
#include "app/xgame/MsgSetPosition.h"
#include "app/xgame/MsgSpringImpulse.h"
#include "app/xgame/MsgStayTrigger.h"
#include "app/xgame/MsgStopGameTimer.h"
#include "app/xgame/MsgTakeObject.h"
#include "app/xgame/MsgWarpNewArea.h"

// hh
#include "hh/base.h"

// hh:ut
#include "hh/ut/Packfile.h"

// SurfRide
#include "SurfRide/Layer.h"

// Yoshi Objects
#include "Yoshi Objects/EggManager.h"
#include "Yoshi Objects/EnemyShyGuy.h"
#include "Yoshi Objects/ObjCrayPipe.h"
#include "Yoshi Objects/ObjCrayPipeExit.h"
#include "Yoshi Objects/ObjRotateLift.h"
#include "Yoshi Objects/ObjYoshiCoin.h"
#include "Yoshi Objects/ObjYoshiJumpBoard.h"
#include "Yoshi Objects/ObjYoshiJumpBoardBig.h"
#include "Yoshi Objects/ObjYoshiJumpBoardSmall.h"
#include "Yoshi Objects/ObjYoshiSpecialFlower.h"

#include "app/Objects/CSharedObjInfo.h"

#include "app/HUD/CHudGameMainDisplay.h"