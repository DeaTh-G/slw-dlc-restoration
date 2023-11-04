#pragma once

namespace slw_dlc_restoration
{
	class ObjCrayPipe : public app::ObjCrayPipe
	{
	public:
		typedef TiFsmState_t(app::ObjCrayPipe::* BaseState)(const TiFsmEvent_t&);

	public:
		int NumPlayers{};

		void AddCallback(app::GameDocument* in_pDocument) override;
		void Update(const app::fnd::SUpdateInfo& in_rUpdateInfo) override;
		TiFsmState_t StatePipeIn(const TiFsmEvent_t& in_rEvent);
	};

	class ObjCrayPipeExit : public app::ObjCrayPipeExit
	{
	public:
		typedef TiFsmState_t(app::ObjCrayPipeExit::* BaseState)(const TiFsmEvent_t&);

	public:
		bool AllPlayersCaught{};
		int NumPlayers{};

		ObjCrayPipeExit();
		void AddCallback(app::GameDocument* in_pDocument) override;
		TiFsmState_t StateIdle(const TiFsmEvent_t& in_rEvent);
		TiFsmState_t StatePipeOut(const TiFsmEvent_t& in_rEvent);
	};
}