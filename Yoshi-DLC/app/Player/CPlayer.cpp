#include "pch.h"
#include "CPlayer.h"

HOOK(int*, __fastcall, UpdatePlayerInformationHook, ASLR(0x00851F20), int* This)
{
	int* result = originalUpdatePlayerInformationHook(This);
	
	void* service = app::xgame::Blackboard::GetService();
	int* blackBoard = app::xgame::Blackboard::GetPlayerInformation(This, service);
	
	/* Store IsMovingGround because Sonic Team felt like yeeting that shit. */
	if (*((int*)This[0xD2] + 9) >= 0)
	{
		int* playerInfo = *(int**)(blackBoard + 1 * *((int*)This[0xD2] + 9) + 0xF);

		if (playerInfo)
		{
			int* cStateGOC = app::CGOCCollectionImpl::GetGOC((void*)(This + 0xCB), CStateGOC);
			unsigned int isMovGround = app::Player::StateUtil::IsLandOnMovableGround(cStateGOC);
			*((char*)playerInfo + 0x145) = isMovGround;
		}
	}

	return result;
}

void app::Player::CPlayer::UpdatePlayerInformation()
{
	INSTALL_HOOK(UpdatePlayerInformationHook);
}
