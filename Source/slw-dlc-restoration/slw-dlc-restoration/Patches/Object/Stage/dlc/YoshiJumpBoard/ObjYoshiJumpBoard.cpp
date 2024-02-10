#include "pch.h"
#include "ObjYoshiJumpBoard.h"

slw_dlc_restoration::ObjYoshiJumpBoard::ObjYoshiJumpBoard(app::yoshi_jump_board::JumpBoardType in_type) : app::ObjYoshiJumpBoard(in_type)
{

}

void slw_dlc_restoration::ObjYoshiJumpBoard::AddCallback(app::GameDocument* in_pDocument)
{
	app::ObjYoshiJumpBoard::AddCallback(in_pDocument);

	if (auto* pShadowSimple = GetComponent<app::game::GOCShadowSimple>())
		pShadowSimple->SetVisible(false);
}

app::CSetObjectListener* slw_dlc_restoration::ObjYoshiJumpBoard::Create_Big()
{
	return new ObjYoshiJumpBoard(app::yoshi_jump_board::JumpBoardType::Big);
}

app::CSetObjectListener* slw_dlc_restoration::ObjYoshiJumpBoard::Create_Small()
{
	return new ObjYoshiJumpBoard(app::yoshi_jump_board::JumpBoardType::Small);
}
