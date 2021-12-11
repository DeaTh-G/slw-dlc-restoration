#pragma once

namespace app
{
    namespace game
    {
        class alignas(16) MoveObjGolonRock : public MoveController
        {
        public:
            struct SetupParam
            {
                csl::math::Vector3 Position{};
                float field_10{};
                float field_14{};
                float field_18{};
                float Speed{};
                float field_20{};
                float field_24{};
                float field_28{};
                bool field_2C{};
                INSERT_PADDING(3);
                float field_30{};
                float field_34{};
                bool IsCheckFall{};
                INSERT_PADDING(3);
                short field_3C{};
                short field_3E{};
                void* NotifyMoveEndCallback{};
                short field_44{};
                short field_46{};
                void* NotifyPassPlayerCallback{};
                int field_4C{};
            };

            enum class Mode : char
            {
            };

        private:
            float field_10{};
            float field_14{};
            float field_18{};
            float field_1C{};
            float field_20{};
            float field_24{};
            bool field_28{};
            INSERT_PADDING(3);
            float field_2C{};
            float field_30{};
            bool IsCheckFall{};
            INSERT_PADDING(3);
            Mode Mode{};
            INSERT_PADDING(3);
            PathEvaluator PathEvaluator{};
            float field_4C{};
            float field_50{};
            float field_54{};
            float field_58{};
            int field_5C{};
            csl::math::Vector3 field_60{};
            CPhysicsWorld* PhysicsWorld{};
            PhysicsRaycastJob* RaycastJob{};
            float field_78{};
            float field_7C{};
            short field_80{};
            short field_82{};
            void* NotifyMoveEndCallback{};
            short field_88{};
            short field_8A{};
            void* NotifyPassPlayerCallback{};
            int field_90{};
            int field_94{};
            int field_98{};
            int field_9C{};

        protected:
            int Update(const fnd::SUpdateInfo& updateInfo) override 
            {
                GOCMovement* gocMovement = GetOwnerMovement();
                int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
                if (!IsCheckFall)
                {
                    field_58 *= updateInfo.deltaTime;
                    float someFloat = -((((field_4C + field_4C) / field_10) * field_58) - field_50);
                    if (field_58 <= field_10)
                    {
                        csl::math::Matrix34 matrix{};
                        csl::math::Vector3 splinePoint{};
                        csl::math::Vector3 someVector{};
                        csl::math::Vector3 someVector2{};
                        game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);

                        Eigen::Vector3f v(someVector2.X, someVector2.Y, someVector2.Z);
                        Eigen::Matrix3f m(Eigen::AngleAxisf(field_54, v));
                        for (size_t i = 0; i < 3; i++)
                            for (size_t j = 0; j < 3; j++)
                                matrix.data[i][j] = m(i, j);

                        someVector = MultiplyMatrixSRByVector(&m, &someVector);
                        math::Vector3Scale(&someVector, field_50 - (field_4C + field_4C), &someVector);
                        math::Vector3Add(&splinePoint, &someVector, &splinePoint);
                        *(csl::math::Vector3*)contextParam = splinePoint;
                        csl::Matrix34OrthonormalDirection(&matrix, &someVector2, &someVector);
                        csl::math::Quaternion rotation = GetRotationFromMatrix(&matrix);
                        *((csl::math::Quaternion*)contextParam + 1) = rotation;
                        
                        return 0;
                    }

                    if (field_14 > 0)
                    {
                        field_14 -= updateInfo.deltaTime;
                        return 0;
                    }
                    
                    field_50 -= (field_4C + field_4C);
                    if (!field_90 || field_82 == 0)
                    {
                        csl::math::Matrix34 matrix{};
                        csl::math::Vector3 splinePoint{};
                        csl::math::Vector3 someVector{};
                        csl::math::Vector3 someVector2{};
                        game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);

                        Eigen::Vector3f v(someVector2.X, someVector2.Y, someVector2.Z);
                        Eigen::Matrix3f m(Eigen::AngleAxisf(field_54, v));
                        for (size_t i = 0; i < 3; i++)
                            for (size_t j = 0; j < 3; j++)
                                matrix.data[i][j] = m(i, j);

                        someVector = MultiplyMatrixSRByVector(&m, &someVector);
                        math::Vector3Scale(&someVector, field_50, &someVector);
                        math::Vector3Add(&splinePoint, &someVector, &splinePoint);
                        *(csl::math::Vector3*)contextParam = splinePoint;
                        csl::Matrix34OrthonormalDirection(&matrix, &someVector2, &someVector);
                        csl::math::Quaternion rotation = GetRotationFromMatrix(&matrix);
                        *((csl::math::Quaternion*)contextParam + 1) = rotation;

                        return 0;
                    }

                    if (field_82 >= 0)
                    {
                        // virtual call
                        // (*(*(v25 + SLOWORD(a1->field_84)) + 8 * v24 + 4))();
                        return 0;
                    }

                    // ObjGolonRock::NotifyMoveEndCallback(v25);
                    return 0;
                }

                if ((char)Mode == 1)
                {
                    if ((char)Mode == 2)
                        return 0;
                
                    csl::math::Vector3 gravityDir{};
                    csl::math::Vector3 vector{};
                    int* gocGravity = GameObject::GetGOC((GameObject*)((int*)gocMovement + 5), GOCGravityString);
                    gravityDir = *GOCGravity::GetGravityDirection(gocGravity);
                    math::Vector3Scale(&gravityDir, field_2C, &gravityDir);
                    math::Vector3Scale(&gravityDir, updateInfo.deltaTime, &vector);
                    *((csl::math::Vector3*)contextParam + 2) += vector;
                    math::Vector3Scale(((csl::math::Vector3*)contextParam + 2), updateInfo.deltaTime, &vector);
                    *(csl::math::Vector3*)contextParam += vector;
                    UpdateLocalRotRad(updateInfo.deltaTime);
                    field_58 *= updateInfo.deltaTime;
                    if (field_58 <= field_30)
                        return 0;

                    if (!field_90)
                        return 0;

                    if (!field_82)
                        return 0;

                    if (field_82 < 0)
                    {
                        // virtual call
                        // ObjGolonRock::NotifyMoveEndCallback(v25);
                        return 0;
                    }

                    // virtual call
                    // v74 = a1->field_90;
                    // v25 = v74 + a1->field_80;
                    // (*(*(v25 + SLOWORD(a1->field_84)) + 8 * v24 + 4))();

                    return 0;
                }

                csl::math::Vector3 movePos{};
                csl::math::Vector3 rotDir{};
                UpdateMovePathPos(&movePos, updateInfo.deltaTime);
                UpdateRotDirPathToPos(&rotDir, updateInfo.deltaTime);

                csl::math::Vector3 scaledRotDir{};
                math::Vector3Scale(&rotDir, field_50, &scaledRotDir);
                math::Vector3Add(&movePos, &scaledRotDir, &movePos);
                *(csl::math::Vector3*)contextParam = movePos;
                PathEvaluator.GetTangent(&scaledRotDir, PathEvaluator.field_08);
                
                csl::math::Matrix34 matrix{};
                csl::Matrix34OrthonormalDirection(&matrix, &scaledRotDir, &movePos);
                csl::math::Quaternion rotation = GetRotationFromMatrix(&matrix);
                *((csl::math::Quaternion*)contextParam + 1) = rotation;
                field_60 = Vector3((field_50 + field_4C) * field_24, 0, field_18 * updateInfo.deltaTime);
                UpdateLocalRotRad(updateInfo.deltaTime);
                if (!IsCheckFall || CheckFall(&rotDir, updateInfo.deltaTime) )
                {
                    if (IsPassOverPlayer())
                    {
                        if (field_90)
                        {
                            if (field_8A)
                            {
                                // v57 = a1->field_90;
                                // v59 = v57 + a1->field_88;
                                if (field_8A >= 0)
                                {
                                    // virtual call
                                    // (*(*(v59 + SLOWORD(a1->field_8C)) + 8 * v58 + 4))();
                                }
                                else
                                {
                                    // ObjGolonRock::NotifyPassPlayerCallback(v59);
                                }
                            }
                        }
                    }
                    return 0;
                }

                if (field_90)
                {
                    if (field_82)
                    {
                        if (field_82 >= 0)
                        {
                            // virtual call
                            // v56 = a1->field_90;
                            // v25 = v56 + a1->field_80;
                            // (*(*(v25 + SLOWORD(a1->field_84)) + 8 * v24 + 4))();
                        }

                        // ObjGolonRock::NotifyMoveEndCallback(v25);
                        return 0;
                    }
                }

                return 0;
            };

            bool CheckFall(csl::math::Vector3* rotDir, float deltaTime)
            {
                bool result{};
                int isFalling[3] { -1, -1, -1 };

                int item = ((int*)RaycastJob->Commands[0])[0];
                for (size_t i = 0; item != ((int*)RaycastJob->Commands[0])[16 * RaycastJob->Commands[1]]; i++)
                {
                    isFalling[i] = 1;
                    if ((int*)item + 12)
                        isFalling[i] = 0;
                    
                    item += 40;
                }
                RaycastJob->Clear();

                csl::math::Vector3 vector{};
                GOCMovement* gocMovement = GetOwnerMovement();
                int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
                if (isFalling[0] == 1 && isFalling[2] != 1)
                {
                    float scalar = csl::math::Select(field_20 * deltaTime, fabs(field_20 * deltaTime), -fabs(field_20 * deltaTime));
                    if (isFalling[1])
                        vector = Vector3(field_50 * scalar, 0, field_18 * deltaTime);
                    else
                        vector = Vector3(field_50 * -scalar, 0, field_18 * deltaTime);

                    math::Vector3Rotate(&vector, (csl::math::Quaternion*)contextParam + 1, &vector);
                    math::Vector3Scale(&vector, 1 / deltaTime, &vector);
                    *((csl::math::Vector3*)contextParam + 2) = vector;
                    result = true;
                }

                csl::math::Vector3 scaledRotDir{};
                math::Vector3Scale(rotDir, field_4C + 2, &scaledRotDir);
                math::Vector3Add((csl::math::Vector3*)contextParam, &scaledRotDir, &scaledRotDir);
                RaycastJob->Add((csl::math::Vector3*)contextParam, &scaledRotDir, 51606, 0, 1);

                if (isFalling[0] != 1)
                {
                    PhysicsWorld->AddRaycastJob(RaycastJob);
                    return result;
                }

                csl::math::Vector3 splinePoint{};
                csl::math::Vector3 someVector{};
                csl::math::Vector3 someVector2{};
                game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);
                float axis[2]{};
                axis[0] = csl::math::Select((field_4C * 5) / (field_50 + field_4C), fabs((field_4C * 5) / (field_50 + field_4C)), -fabs((field_4C * 5) / (field_50 + field_4C)));
                axis[1] = SonicUSA::System::RadianMaskS(field_54 - axis[0]);
                axis[0] = SonicUSA::System::RadianMaskS(field_54 + axis[0]);

                csl::math::Matrix34 matrix{};
                for (size_t i = 0; i < 2; i++)
                {
                    Eigen::Vector3f v(someVector2.X, someVector2.Y, someVector2.Z);
                    Eigen::Matrix3f m(Eigen::AngleAxisf(axis[i], v));
                    for (size_t i = 0; i < 3; i++)
                        for (size_t j = 0; j < 3; j++)
                            matrix.data[i][j] = m(i, j);

                    someVector = MultiplyMatrixSRByVector(&m, &someVector);
                    math::Vector3Scale(&someVector, field_50, &someVector2);
                    math::Vector3Add(&splinePoint, &someVector2, &splinePoint);
                    math::Vector3Scale(&someVector, field_4C + 2, &someVector);
                    math::Vector3Add(&splinePoint, &someVector, &someVector);
                    RaycastJob->Add(&splinePoint, &someVector, 51606, 0, 1);
                }

                PhysicsWorld->AddRaycastJob(RaycastJob);
                return result;
            }

            bool const IsPassOverPlayer()
            {
                // Fix this so it detects P2 as well
                GOCMovement* gocMovement = GetOwnerMovement();
                int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), 0);
                if (!playerInfo)
                    return false;

                csl::math::Vector3 playerPosition{ *(csl::math::Vector3*)(playerInfo + 4) };

                csl::math::Vector3 splinePoint{};
                csl::math::Vector3 someVector{};
                csl::math::Vector3 someVector2{};
                game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);
                float scalar = csl::math::Select(((float*)gocMovement)[6], fabs(1), -abs(1));
                math::Vector3Scale(&someVector2, scalar, &someVector2);
                math::Vector3Subtract(&playerPosition, &splinePoint, &playerPosition);
                float dot = math::Vector3DotProduct(&playerPosition, &someVector);
                math::Vector3Scale(&someVector, dot, &someVector);
                math::Vector3Subtract(&playerPosition, &someVector, &playerPosition);
                if (math::Vector3DotProduct(&someVector2, &playerPosition) >= 0)
                    return false;

                float magnitude;
                math::Vector3SquareMagnitude(&playerPosition, &magnitude);
                if (magnitude > 90000)
                    return true;

                return false;
            }

            void const UpdateLocalRotRad(float deltaTime)
            {
                csl::math::Vector3 vector{ field_60 };
                float length = math::Vector3NormalizeWithLength(&vector, &vector);
                if (length <= 0)
                    field_7C = SonicUSA::System::RadianMaskS(field_7C + (length / field_4C));

                float someValue = 0;
                csl::math::Vector3 someVector{ 0, 0, 1 };
                csl::math::Vector3 someVector2{ 0, 1, 0 };
                vector = field_60;
                math::Vector3RotateY(&someVector, field_78, &someVector);
                float dot = math::Vector3DotProduct(&someVector, &vector);
                dot = csl::math::Clamp(dot, -1, 1);
                dot = acosf(dot);
                if (dot >= (0.52359879f * deltaTime))
                {
                    dot = csl::math::Min(dot, 1.0471976f * deltaTime);
                    someVector = Vector3(0, 1, 0);

                    math::Vector3CrossProduct(&someVector, &someVector2, &someVector);
                    float dot2 = math::Vector3DotProduct(&someVector, &vector);
                    someValue = csl::math::Select(dot2, fabs(dot), -fabs(dot));
                }
                field_78 += someValue;
                field_78 = csl::math::Clamp(field_78, -1.0471976f, 1.0471976f);
                field_7C = SonicUSA::System::RadianMaskS(field_7C + (length / field_4C));
            }

            void const UpdateMovePathPos(csl::math::Vector3* out, float deltaTime)
            {
                csl::math::Vector3 position{};

                float distance = (field_18 * deltaTime) + PathEvaluator.field_08;
                if (distance > 0 || distance < game::PathEvaluator::GetLength(&PathEvaluator))
                    game::PathEvaluator::SetDistance(&PathEvaluator, distance);

                PathEvaluator.GetWorldPosition(&position, PathEvaluator.field_08);
                *out = position;
            }

            csl::math::Vector3 const UpdateRotDirPathToPos(csl::math::Vector3* out, float deltaTime)
            {
                csl::math::Quaternion rotation{};
                csl::math::Vector3 vectorOffset{};
                csl::math::Vector3 vector{};

                csl::math::Vector3 splinePoint{};
                csl::math::Vector3 someVector{};
                csl::math::Vector3 someVector2{};
                game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);

                if (!field_28)
                {
                    Eigen::Vector3f v(someVector.X, someVector.Y, someVector.Z);
                    Eigen::Quaternionf q(Eigen::AngleAxisf(field_54, v));
                    rotation = csl::math::Quaternion(q.y(), q.z(), q.w(), q.x());

                    math::Vector3Rotate(&vector, &rotation, &someVector);
                    return vector;
                }

                // Fix this so it detects P2 as well
                GOCMovement* gocMovement = GetOwnerMovement();
                int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), 0);
                if (!playerInfo)
                    return Vector3(0, 1, 0);

                csl::math::Vector3 playerPosition{ *(csl::math::Vector3*)(playerInfo + 4) };
                math::Vector3Subtract(&playerPosition, &splinePoint, &playerPosition);

                float dot = math::Vector3DotProduct(&playerPosition, &someVector2);
                math::Vector3Scale(&someVector2, dot, &splinePoint);
                math::Vector3Subtract(&playerPosition, &splinePoint, &splinePoint);
                playerPosition = splinePoint;
                if (!math::Vector3NormalizeIfNotZero(&splinePoint, &splinePoint))
                    return Vector3(0, 1, 0);

                Eigen::Vector3f v(someVector.X, someVector.Y, someVector.Z);
                Eigen::Quaternionf q(Eigen::AngleAxisf(field_54, v));
                vectorOffset = csl::math::Vector3(v.x(), v.y(), v.z());
                rotation = csl::math::Quaternion(q.y(), q.z(), q.w(), q.x());

                math::Vector3Rotate(&vector, &rotation, &someVector);
                dot = math::Vector3DotProduct(&vector, &playerPosition);
                dot = csl::math::Clamp(dot, -1, 1);

                float csofDot = acosf(dot);
                float mult = field_1C * deltaTime;

                math::Vector3CrossProduct(&vectorOffset, &vector, &vector);
                if (math::Vector3DotProduct(&playerPosition, &vector) < 0)
                {
                    mult = -mult;
                    csofDot = -csofDot;
                }

                float MULT_field_20 = field_20 * deltaTime;
                field_24 += mult;
                field_24 = csl::math::Clamp(field_24, -MULT_field_20, MULT_field_20);
                if ((field_24 > 0 && mult < 0) || (field_24 < 0 && mult > 0))
                {
                    if (fabs(field_24) > fabs(mult))
                    {
                        float min = csl::math::Min(10 * deltaTime, 1);
                        field_24 = csl::math::Lerp(field_24, csofDot, min);
                    }
                }
                field_54 += field_24;
                field_54 = SonicUSA::System::RadianMaskS(field_54);

                v = Eigen::Vector3f(vectorOffset.X, vectorOffset.Y, vectorOffset.Z);
                q = Eigen::Quaternionf(Eigen::AngleAxisf(field_54, v));
                rotation = csl::math::Quaternion(q.y(), q.z(), q.w(), q.x());

                math::Vector3Rotate(&vector, &rotation, &someVector);
                return vector;
            }

        public:
            void Setup(SetupParam& param)
            {
                GameDocument* document = (GameDocument*)(((int***)GetOwnerMovement())[5][9][1]);

                int i = 5;
                do
                {
                    ++field_14 = ++param.field_18;
                    --i;
                } while (i);

                field_2C = param.field_30;
                field_30 = param.field_34;
                IsCheckFall = param.IsCheckFall;
                PhysicsWorld = (CPhysicsWorld*)CPhysicsWorld::GetService(document);
                RaycastJob = new game::PhysicsRaycastJob[1];
                field_80 = param.field_3C;
                NotifyMoveEndCallback = param.NotifyMoveEndCallback;
                field_88 = param.field_44;
                NotifyPassPlayerCallback = param.NotifyPassPlayerCallback;
                field_90 = param.field_4C;

                int* gravityField = ObjUtil::GetGravityField(document, &param.Position);
                if (!gravityField)
                    return;
            
                int* path = ObjUtil::GetGravityPathInsideCylinder(gravityField);
                if (!path)
                    return;

                PathEvaluator::SetPathObject(&PathEvaluator, path);
                float length = PathEvaluator::GetLength(&PathEvaluator);

                csl::math::Vector3 position{};
                game::PathEvaluator::GetClosestPositionAlongSpline(&PathEvaluator, &param.Position, &position, 0, &length);
                game::PathEvaluator::SetDistance(&PathEvaluator, position.X);
                csl::math::Vector3 splinePoint{};
                csl::math::Vector3 someVector{};
                csl::math::Vector3 someVector2{};
                game::PathEvaluator::GetPNT(&PathEvaluator, PathEvaluator.field_08, &splinePoint, &someVector, &someVector2);
                math::Vector3Subtract(&param.Position, &splinePoint, &splinePoint);
                field_50 = math::Vector3Magnitude(&splinePoint);
                if (!math::Vector3NormalizeIfNotZero(&splinePoint, &splinePoint))
                    return;
            
                float dot = math::Vector3DotProduct(&someVector, &splinePoint);
                dot = acosf(csl::math::Clamp(dot, -1, 1));
                math::Vector3CrossProduct(&someVector2, &someVector, &someVector);
                if (math::Vector3DotProduct(&someVector, &splinePoint) < 0)
                    dot = -dot;
            
                field_54 = dot;
            }

            csl::math::Quaternion const GetLocalRotate(csl::math::Quaternion* rotation)
            {
                csl::math::Vector3 upVector { 0, 1, 0 };
                csl::math::Vector3 backwardVector { -1, 0, 0 };

                csl::math::Quaternion rotation1{};
                csl::math::Quaternion rotation2{};

                Eigen::Vector3f v(upVector.X, upVector.Y, upVector.Z);
                Eigen::Quaternionf q(Eigen::AngleAxisf(field_78, v));
                rotation1 = csl::math::Quaternion(q.y(), q.z(), q.w(), q.x());

                v = Eigen::Vector3f(backwardVector.X, backwardVector.Y, backwardVector.Z);
                q = Eigen::Quaternionf(Eigen::AngleAxisf(field_7C, v));
                rotation2 = csl::math::Quaternion(q.y(), q.z(), q.w(), q.x());

                rotation1 = csl::math::Quaternion(rotation1.X * rotation2.X, rotation1.Y * rotation2.Y,
                    rotation1.Z * rotation2.Z, rotation1.Z * rotation2.Z);

                csl::math::QuaternionNormalize(&rotation1, rotation);
                return *rotation;
            }

            csl::math::Vector3 const GetWorldMoveDir()
            {
                csl::math::Vector3 worldDir{};

                GOCMovement* gocMovement = GetOwnerMovement();
                int* contextParam = game::GOCMovement::GetContextParam((int*)gocMovement);
                math::Vector3Rotate(&worldDir, (csl::math::Quaternion*)(contextParam + 4), &field_60);
                if (math::Vector3NormalizeIfNotZero(&worldDir, &worldDir))
                    return worldDir;
                
                PathEvaluator.GetTangent(&worldDir, PathEvaluator.field_08);
                float scalar = csl::math::Select(((float*)gocMovement)[6], fabs(1), -abs(1));
                math::Vector3Scale(&worldDir, scalar, &worldDir);

                return worldDir;
            }
        };
    }
}