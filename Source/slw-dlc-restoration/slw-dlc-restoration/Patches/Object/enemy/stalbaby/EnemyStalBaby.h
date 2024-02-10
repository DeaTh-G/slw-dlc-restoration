#pragma once

namespace slw_dlc_restoration
{
	class EnemyStalBaby : public app::EnemyStalBaby
	{
	private:
		inline static const char* ms_pScoreName = "STALBABY";

	public:
		uint KillingPlayerNo{ static_cast<uint>(-1) };

		EnemyStalBaby();
		bool ProcessMessage(app::fnd::Message& in_rMessage) override;
		bool ProcMsgDamage(app::xgame::MsgDamage& in_rMessage);
		bool ProcMsgKick(app::xgame::MsgKick& in_rMessage);

		class State
		{
		public:
			class Dead : public app::EnemyStalBaby::State::Dead
			{
			private:
				inline static const char* ms_pHeadNodeName = "head";

			public:
				Dead() { }

				bool Step(context_type& in_rObj, float in_deltaTime) override;
			};
		};
	};
}