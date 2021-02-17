#pragma once

namespace app
{
	HOOK(void, __fastcall, ProcMsgHitEventCollisionHook, ASLR(0x00662320), int* This, void* edx, int* message)
	{
		const char* packFileName = ObjUtil::GetStagePackName(*Document);
		if (strncmp(packFileName, "zdlc02", 6) == 0)
		{
			MessageTakeObject msgTakeObject;
			game::EffectCreateInfo effectInfo;

			int* gocSound;
			int* gocEffect;
			int something[3];
			int effectVar = 0;

			if ((This[0xD2] & 0x40) == 0)
			{
				xgame::MsgTakeObject::__ct(&msgTakeObject, 0);
				xgame::MsgTakeObject::SetShapeUserID(&msgTakeObject, *(int*)(message[7] + 0xBC));
				if (fnd::CActor::SendMessageImm((int*)This + 2, message[8], (int*)&msgTakeObject))
				{
					if (This[0xCA])
						game::GOCEffect::CreateEffect((int*)This[0xCA], "ef_dl2_coin_get");

					gocSound = GameObject::GetGOC((GameObject*)This, GOCSoundString);
					if (gocSound)
						game::GOCSound::Play(gocSound, something, "obj_yossycoin_get", 0);

					int playerNo = ObjUtil::GetPlayerNo(This[0xA], message[8]);
					if ((This[0xD2] & 4) != 0)
						ObjUtil::AddScorePlayerAction((GameObject*)This, "GET_DROPPED_YOSHICOIN", playerNo);
					else
						ObjUtil::AddScorePlayerAction((GameObject*)This, "GET_YOSHICOIN", playerNo);

					This[0xD2] |= 0x40;
					GameObject::Kill((GameObject*)This);
				}
				xgame::MsgExtendPlayer::__dt((int*)&msgTakeObject);
			}
		}
		else
		{
			originalProcMsgHitEventCollisionHook(This, edx, message);
		}
	}

	class CObjDroppedRing
	{
	public:
		static void ProcMsgHitEventCollision()
		{
			INSTALL_HOOK(ProcMsgHitEventCollisionHook);
		}
	};
}