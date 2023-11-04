#pragma once

namespace slw_dlc_restoration
{
	class ObjYoshiJumpBoard : public app::ObjYoshiJumpBoard
	{
	public:
		ObjYoshiJumpBoard(app::yoshi_jump_board::JumpBoardType in_type);
		void AddCallback(app::GameDocument* in_pDocument) override;
		static CSetObjectListener* Create_Big();
		static CSetObjectListener* Create_Small();
	};
}