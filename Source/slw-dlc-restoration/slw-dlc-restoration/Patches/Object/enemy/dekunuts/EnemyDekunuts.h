#pragma once

namespace slw_dlc_restoration
{
	class EnemyDekunuts : public app::EnemyDekunuts
	{
	public:
		uint PlayerNo{ static_cast<uint>(-1) };

	private:
		inline static size_t ms_StateCount = 5;
		inline static int ms_DefaultState = 0;

	public:
		EnemyDekunuts();
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgLeaveEventCollision(app::xgame::MsgLeaveEventCollision& in_rMessage);
		bool ProcMsgStayTrigger(app::xgame::MsgStayTrigger& in_rMessage);
		void RotateTarget(float in_unk1, float in_unk2);

		class State
		{
		public:
			class Appear : public app::EnemyDekunuts::State::Appear
			{
			public:
				Appear() { }
				
				bool Step(context_type& in_rObj, float in_deltaTime) override;
			};

			class Attack : public  app::EnemyDekunuts::State::Attack
			{
			public:
				Attack() { }

				bool Step(context_type& in_rObj, float in_deltaTime) override;
			};
		};
	};
}