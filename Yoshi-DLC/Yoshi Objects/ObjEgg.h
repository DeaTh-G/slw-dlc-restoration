#pragma once

namespace app
{
	class ObjEgg;

	struct EggCInfo
	{
		csl::math::Matrix34* Transform;
		int ModelType;
	};

	namespace egg
	{
		ObjEgg* CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo);
	}

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
		INSERT_PADDING(0x1C);
		EggCInfo CInfo;
		int ModelType;
		INSERT_PADDING(0x40);
	
	public:
		ObjEgg(GameDocument& gameDocument, EggCInfo* cInfo) {}

		void AddCallback(GameDocument* gameDocument) override
		{
			fnd::GOComponent::Create(this, GOCVisualModel);
			fnd::GOComponent::Create(this, GOCCollider);
			fnd::GOComponent::Create(this, GOCGravity);
			fnd::GOComponent::Create(this, GOCEffect);
			fnd::GOComponent::Create(this, GOCSound);
		}
	};
}

app::ObjEgg* app::egg::CreateEgg(GameDocument& gameDocument, EggCInfo* cInfo)
{
	ObjEgg* object = new ObjEgg(gameDocument, cInfo);
	if (!object)
		return 0;
	GameDocument::AddGameObject(gameDocument, object);
	return object;
}