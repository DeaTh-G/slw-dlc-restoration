#pragma once

namespace slw_dlc_restoration
{
	class ObjTreasureBox : public app::ObjTreasureBox
	{
	protected:
		inline static const float ms_ItemGetTwinkleEffectScale = 0.7f / 0.81f;
		inline static const csl::math::Vector3 ms_ItemScale = { 0.7f / 0.9f, 0.7f / 0.9f, 0.7f / 0.9f };
		inline static const csl::math::Vector3 ms_PositionOffset = { 0.0f, 11.2f / 0.9f, 13.0f / 0.9f };
		inline static const csl::math::Vector3 ms_CollisionSize = { 9.5f / 0.9f, 8.6f / 0.9f, 7.75f / 0.9f };
		inline static const csl::math::Vector3 ms_TwinklePositionOffset = { 0.0f, 11.2f / 0.9f, 13.0f / 0.9f };

	public:
		typedef TiFsmState_t(app::ObjTreasureBox::*BaseState)(const TiFsmEvent_t&);

	public:
		uint InteractingPlayerNo{ static_cast<uint>(-1) };

	public:
		void AddCallback(app::GameDocument* in_pDocument) override;
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		bool IsPlayerPhantom(uint in_playerNo);
		void PushCamera();
		void PopCamera();
		TiFsmState_t StateWait(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateHitOff(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateOpen_ControlCamera(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateOpen_End(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StateOpened(const TiFsmEvent_t& in_rEvent);
	};
}