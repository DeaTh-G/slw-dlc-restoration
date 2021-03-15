#pragma once

namespace app
{
    class ObjEgg;

    class EggManager : public fnd::GameService
    {
    public:
        struct LocusData
        {
            csl::math::Vector3 Position;
            csl::math::Quaternion Rotation;
            char field_20;
            INSERT_PADDING(13);
        };

        std::vector<ObjEgg*> Eggs;
        boost::circular_buffer<LocusData> EffectData;
        INSERT_PADDING(4);
        float field_64{};
        char IsSpaceShrink{};
        char FlowerCount{};
        INSERT_PADDING(2);

        EggManager()
        {
            // TODO: CHECK IF CRASH
            //Locus.pAllocator = (*(csl::fnd::IAllocator**)ASLR(0x00FD3FC4) + 3);
        }

    private:
        static void* staticClass() { return (void*)ASLR(0x00FEE764); }

        void UpdateLocusPos()
        {
            int* playerInfo = ObjUtil::GetPlayerInformation(Document, 0);
            if (!playerInfo)
                return;

            if (EffectData.size() <= 1)
            {
                LocusData data { *(csl::math::Vector3*)(playerInfo + 4),
                    *(csl::math::Quaternion*)(playerInfo + 8), *(playerInfo + 0x51) ^ 1 };

                EffectData.push_front(data);
            }

            LocusData data{ *(csl::math::Vector3*)(playerInfo + 4),
                *(csl::math::Quaternion*)(playerInfo + 8), 0 };

            EffectData.push_front(data);
        }

        char SubSpaceCount();

        void UpdateEggSpace()
        {
            IsSpaceShrink &= ~4;
            int* playerInfo = ObjUtil::GetPlayerInformation(Document, 0);
            if (playerInfo && *(char*)(playerInfo + 0x51))
                SubSpaceCount();

            if (IsSpaceShrink & 1)
                SubSpaceCount();

            if (field_64 > 0)
                AddSpaceCount();
        }

    public:
        bool AddEgg(ObjEgg* egg)
        {
            if (Eggs.size() >= 0xC)
                return false;
            
            Eggs.push_back(egg);
            return true;
        }

        bool CheckAccessLocusData(LocusData* locus)
        {
            if (!EffectData.size())
                return false;

            LocusData data = EffectData.at(20);
            *locus = data;

            return true;
        }

        void GetTargetDataFromLocusIndex(LocusData* locus, bool a2, float* magnitude)
        {
            LocusData data1{};
            LocusData data2{};

            if (!CheckAccessLocusData(&data1) || a2 || !CheckAccessLocusData(&data2))
                return;

            *locus = data1;

            csl::math::Vector3 positionDifference{};
            float localMagnitude{};
            math::Vector3Subtract(&data1.Position, &data2.Position, &positionDifference);
            math::Vector3Magnitude(&positionDifference, &localMagnitude);

            if (localMagnitude > 0.3f && (field_64 > 0 || (IsSpaceShrink >> 2) & 1))
            {
                a2 = true;
                if (magnitude)
                    *magnitude = localMagnitude;
            }
        }
        
        void SetForceSpaceShrink(bool isShrink)
        {
            if (isShrink)
                IsSpaceShrink |= 2;
            else
                IsSpaceShrink &= ~2;
        }

        char AddSpaceCount();

        void TakeYoshiSpecialFlower(char flowerID) { FlowerCount |= 1 << flowerID; }
        unsigned int IsYoshiSpecialFlowerTaked(char flowerID) { return (FlowerCount >> flowerID) & 1; }

        static EggManager* GetService(GameDocument* gameDocument)
        {
            void* managerClass = staticClass();
            EggManager* service = (EggManager*)GameDocument::GetServiceByClass(gameDocument, managerClass);
            return service;
        }

        void StartGame(bool a1) override
        {
            EffectData.clear();
            EffectData.set_capacity(egg::GetMaxLocusPositionNum());
            Eggs.clear();
        
            field_64 = 0;
            IsSpaceShrink = false;
            FlowerCount = 0;
        }

        void Update(const fnd::SUpdateInfo& updateInfo) override
        {
            UpdateLocusPos();
            UpdateEggSpace();
        }
    };
    
    inline static EggManager* construct_EggManager(csl::fnd::IAllocator* pAllocator)
    {
        return new(pAllocator) EggManager();
    }
}