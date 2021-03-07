#pragma once

namespace app
{
	class ObjEggInfo
	{
	public:
		INSERT_PADDING(12);
		int Models[4]{};

		virtual ~ObjEggInfo() {};
		virtual void Load(int* loader) {};
		virtual void Initialize(GameDocument* gameDocument)
		{
			const char* names[4] { "zdlc02_obj_yoshiegg_green", "zdlc02_obj_yoshiegg_blue", "zdlc02_obj_yoshiegg_red", "zdlc02_obj_yoshiegg_yellow" };

			int packFile = 0;
			ObjUtil::GetPackFile(&packFile, ObjUtil::GetStagePackName(gameDocument));

			for (size_t i = 0; i < 4; i++)
				ObjUtil::GetModelResource(&Models[i], names[i], &packFile);
		}
		virtual void Finalize() {};
		virtual const char* GetInfoName()
		{
			return "ObjEggInfo";
		}
		virtual void RegistCallback(int* container) {};
	};
}