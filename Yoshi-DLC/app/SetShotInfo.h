#pragma once
#include "../framework.h"
#include "../LWTypes.h"
#include "game/GOCLauncher.h"

namespace app
{
	void SetShotInfo(float speed, float distance, float outOfControl, Matrix34* transformMatrix, bool jumpBoardType, app::game::GOCLauncher::ShotInfo* shotInfo)
	{
		shotInfo->StartingPosition.X = transformMatrix->data[3][0];
		shotInfo->StartingPosition.Y = transformMatrix->data[3][1];
		shotInfo->StartingPosition.Z = transformMatrix->data[3][2];
		shotInfo->LaunchVector.Y = speed;
		shotInfo->OutOfControl = outOfControl;
		shotInfo->TravelTime = distance / speed;
	}
}