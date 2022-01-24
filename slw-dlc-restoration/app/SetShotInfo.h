#pragma once

namespace app
{
    inline static void SetShotInfo(float speed, float distance, float outOfControl, csl::math::Matrix34* transformMatrix, bool jumpBoardType, game::GOCLauncher::ShotInfo* shotInfo)
    {
        shotInfo->StartingPosition.X = transformMatrix->data[3][0];
        shotInfo->StartingPosition.Y = transformMatrix->data[3][1];
        shotInfo->StartingPosition.Z = transformMatrix->data[3][2];
        shotInfo->LaunchVector.Y = speed;
        shotInfo->OutOfControl = outOfControl;
        shotInfo->TravelTime = distance / speed;
    }
}