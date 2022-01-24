#pragma once

namespace app
{
    namespace Camera
    {
        class TreasureBoxCamera : public CCameraController
        {
        private:
            void OnEnter(EnterEventArgs& args) override { IsActive = true; }

        public:
            void SetFovy(float fovy) { CCameraController::SetFovy(fovy); }
            void SetCameraParameter(const csl::math::Vector3& position, const csl::math::Vector3& a2, const csl::math::Vector3& targetPosition)
            {
                CCameraController::SetCameraParam(position, a2, targetPosition);
            }
        };
    }
}   