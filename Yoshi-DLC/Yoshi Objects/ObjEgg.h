#pragma once

namespace app
{
	struct EggCInfo;
	namespace egg
	{
		ObjEgg* CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo);
	}

	struct EggCInfo
	{
		csl::math::Matrix34* Transform;
		int ModelType;
	};

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

	class ObjEgg : public GameObject3D
	{
		int State{};
		INSERT_PADDING(0x4);
		INSERT_PADDING(0x14); // TinyFsm
		EggCInfo* CInfo = new EggCInfo();
		int ModelType{};
		int field_33C{};
		int field_340{};
		int field_344{};
		int field_348{};
		int field_34C{};
		csl::math::Vector3 field_350{};
		int field_360{};
		int field_364{};
		int field_368{};
		int field_36C{};
		int field_370{};
		int field_374{};
		int field_378{};
		int field_37C{};
	
	public:
		ObjEgg(GameDocument& gameDocument, EggCInfo* cInfo)
		{
			CInfo = cInfo;
			ModelType = cInfo->ModelType;
		}

		void AddCallback(GameDocument* gameDocument) override
		{
			sizeof(ObjEgg);
			fnd::GOComponent::Create(this, GOCVisualModel);
			fnd::GOComponent::Create(this, GOCCollider);
			fnd::GOComponent::Create(this, GOCGravity);
			fnd::GOComponent::Create(this, GOCEffect);
			fnd::GOComponent::Create(this, GOCSound);

			EggManager* eggManager = EggManager::GetService(gameDocument);
			if (!eggManager)
				return;
		}
	};
}

app::ObjEgg* app::egg::CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo)
{
	ObjEgg* object = new ObjEgg(gameDocument, cInfo);
	if (!object)
		return 0;
	GameDocument::AddGameObject(*(GameDocument**)&gameDocument, object);
	return object;
}