#pragma once

namespace slw_dlc_restoration
{
	class MoveObjCocco : public app::MoveObjCocco
	{
	public:
		uint PlayerNo{ static_cast<uint>(-1) };

		MoveObjCocco();
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		void SetRelativeTargetPoint(const csl::math::Vector3& in_rTargetPoint, float in_speed, uint in_playerNo);
		void SetTargetPlayer(float in_speed, float in_velocity, uint in_playerNo);
	};
}