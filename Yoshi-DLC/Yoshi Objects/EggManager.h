#pragma once
#include "../app/GameDocument.h"
#include "../csl/fnd.h"
#include "../app/fnd/ReferencedObject.h"
#include "../app/fnd/GameService.h"
#include "../app/fnd/GameServiceClass.h"

csl::fnd::IAllocator* LocusDataAllocator = 0;

namespace app
{
	class EggManager
	{
	public:
		int field_00;
		int field_04;
		int field_08;
		int field_0C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		int field_58;
		int field_5C;
		int field_60;
		int field_64;
		char field_68;
		char field_69;
		char field_6A;
		char field_6B;
		
	private:
		static void* staticClass()
		{
			return (void*)ASLR(0x00FEE764); sizeof(EggManager);
		}

	public:
		static void SetForceSpaceShrink(void* This, bool isShrink)
		{
			bool* isShrinkModeOn = (bool*)This + 0x68;

			if (!*isShrinkModeOn)
				*isShrinkModeOn = true;
			else
				*isShrinkModeOn = false;
		}

		static void TakeYoshiSpecialFlower(void* This, char flowerID)
		{
			((EggManager*)This)->field_69 |= (char)(1 << flowerID);
		}

		static unsigned int IsYoshiSpecialFlowerTaked(void* This, char flowerID)
		{
			return (((EggManager*)This)->field_69 >> flowerID) & 1;
		}

		static void* GetService(GameDocument* gameDocument)
		{
			void* managerClass = staticClass();
			void* service = GameDocument::GetServiceByClass(gameDocument, managerClass);
			return service;
		}
	};
}

app::EggManager* construct_EggManager(csl::fnd::IAllocator* allocator, char a2)
{
	app::EggManager* newObject = (app::EggManager*)app::fnd::ReferencedObject::New(0x6C, allocator);
	if (!newObject)
		return 0;

	app::fnd::GameService::__ct((int*)newObject, 0);
	newObject->field_00 = ASLR(0x00DE0A90);
	newObject->field_0C = ASLR(0x00DE0A74);

	/* ObjEgg */
	newObject->field_3C = 0;
	newObject->field_40 = 0;
	newObject->field_44 = 0;
	newObject->field_48 = 0;

	/* LocusData */
	newObject->field_4C = 0;
	newObject->field_50 = 0;
	newObject->field_54 = 0;
	newObject->field_58 = 0;
	newObject->field_5C = 0;
	*(csl::fnd::IAllocator*)(newObject->field_60) = *LocusDataAllocator;

	newObject->field_64 = 0;

	/* Force Space Shrink */
	newObject->field_68 = 0;

	/* Special Flower Count */
	newObject->field_69 = 0;
	
	return newObject;
}