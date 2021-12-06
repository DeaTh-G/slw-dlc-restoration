#pragma once

namespace app
{
    namespace game
    {
        class alignas(16) MoveObjGolonRock : public MoveController
        {
        public:
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
            bool field_34{};
            INSERT_PADDING(3);
            Mode field_38{};
            INSERT_PADDING(3);
            PathEvaluator PathEvaluator{};
            float field_4C{};
            float field_50{};
            float field_54{};
            float field_58{};
            int field_5C{};
            csl::math::Vector3 field_60{};
            int* PhysicsRaycastJob{};
            float field_78{};
            float field_7C{};
            short field_80{};
            short field_82{};
            int field_84{};
            short field_88{};
            short field_8A{};
            int field_8C{};
            int field_90{};
            int field_94{};
            int field_98{};
            int field_9C{};

        public:
            csl::math::Quaternion const GetLocalRotate(csl::math::Quaternion* rotation)
            {
                csl::math::Vector3 upVector { 0, 1, 0 };
                csl::math::Vector3 backwardVector { -1, 0, 0 };

                csl::math::Quaternion rotation1{};
                csl::math::Quaternion rotation2{};

                Eigen::Vector3f v(upVector.X, upVector.Y, upVector.Z);
                Eigen::Quaternionf q(Eigen::AngleAxisf(field_78, v));
                rotation1 = csl::math::Quaternion(q.y(), q.z(), q.w(), q.x());

                Eigen::Vector3f v(backwardVector.X, backwardVector.Y, backwardVector.Z);
                Eigen::Quaternionf q(Eigen::AngleAxisf(field_7C, v));
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

            bool const IsPassOverPlayer()
            {
                // Fix this so it detects P2 as well
                GOCMovement* gocMovement = GetOwnerMovement();
                int* playerInfo = ObjUtil::GetPlayerInformation((GameDocument*)(((GameObject*)(((int*)gocMovement)[5]))->field_24[1]), 0);
                if (!playerInfo)
                    return false;

                csl::math::Vector3 playerPosition { *(csl::math::Vector3*)(playerInfo + 4) };

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

            void const UpdateLocalRotRad(float a1)
            {
                csl::math::Vector3 vector { field_60 };
                float length = math::Vector3NormalizeWithLength(&vector, &vector);
                if (length <= 0)
                    field_7C = SonicUSA::System::RadianMaskS(field_7C + (length / field_4C));

                float someValue = 0;
                csl::math::Vector3 someVector { 0, 0, 1 };
                csl::math::Vector3 someVector2 { 0, 1, 0 };
                vector = field_60;
                math::Vector3RotateY(&someVector, field_78, &someVector);
                float dot = math::Vector3DotProduct(&someVector, &vector);
                dot = csl::math::Clamp(dot, -1, 1);
                dot = acosf(dot);
                if (dot >= (0.52359879f * a1))
                {
                    dot = csl::math::Min(dot, 1.0471976f * a1);
                    someVector = Vector3(0, 1, 0);

                    math::Vector3CrossProduct(&someVector, &someVector2, &someVector);
                    float dot2 = math::Vector3DotProduct(&someVector, &vector);
                    someValue = csl::math::Select(dot2, fabs(dot), -fabs(dot));
                }
                field_78 += someValue;
                field_78 = csl::math::Clamp(field_78, -1.0471976f, 1.0471976f);
                field_7C = SonicUSA::System::RadianMaskS(field_7C + (length / field_4C));
            }

            void const UpdateMovePathPos(csl::math::Vector3* out, float a2)
            {
                csl::math::Vector3 position{};

                float distance = (field_18 * a2) + PathEvaluator.field_08;
                if (distance > 0 || distance < game::PathEvaluator::GetLength(&PathEvaluator))
                    game::PathEvaluator::SetDistance(&PathEvaluator, distance);

                PathEvaluator.GetWorldPosition(&position, PathEvaluator.field_08);
                *out = position;
            }

            csl::math::Vector3 const UpdateRotDirPathToPos(csl::math::Vector3* out, float a2)
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

                csl::math::Vector3 playerPosition { *(csl::math::Vector3*)(playerInfo + 4) };
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
                float mult = field_1C * a2;

                math::Vector3CrossProduct(&vectorOffset, &vector, &vector);
                if (math::Vector3DotProduct(&playerPosition, &vector) < 0)
                {
                    mult = -mult;
                    csofDot = -csofDot;
                }

                float MULT_field_20 = field_20 * a2;
                field_24 += mult;
                field_24 = csl::math::Clamp(field_24, -MULT_field_20, MULT_field_20);
                if ((field_24 > 0 && mult < 0) || (field_24 < 0 && mult > 0))
                {
                    if (fabs(field_24) > fabs(mult))
                    {
                        float min = csl::math::Min(10 * a2, 1);
                        field_24 = csl::math::Lerp(field_24, csofDot, min);
                    }
                }
                field_54 += field_24;
                field_54 = SonicUSA::System::RadianMaskS(field_54);

                Eigen::Vector3f v(vectorOffset.X, vectorOffset.Y, vectorOffset.Z);
                Eigen::Quaternionf q(Eigen::AngleAxisf(field_54, v));
                rotation = csl::math::Quaternion(q.y(), q.z(), q.w(), q.x());

                math::Vector3Rotate(&vector, &rotation, &someVector);
                return vector;
            }
        };
    }
}