#pragma once

enum class PlayType : int
{
    DEFAULT,
    ALWAYS,
    NEVER
};

extern bool DisablePipeTransition;
extern bool IsConsistentShadow;
extern PlayType LinkSonicPlayType;
extern bool IsLinkSonicFixed;
extern bool IsVirtualLinkSonic;
extern bool IsAlwaysHeartLife;

static csl::math::Quaternion GetRotationFromMatrix(csl::math::Matrix34* matrix)
{
    Eigen::Matrix3f m;

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            m(i, j) = matrix->data[i][j];
    Eigen::Quaternionf q(m.transpose());

    return csl::math::Quaternion(q.x(), q.y(), q.z(), q.w());
}

static csl::math::Vector3 MultiplyMatrixByVector(csl::math::Matrix34* matrix, csl::math::Vector3* vector)
{
    Eigen::Matrix4f m;
    m.setIdentity();
    Eigen::Vector4f v;

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 3; j++)
            m(j, i) = matrix->data[i][j];

    v = m * Eigen::Vector4f(vector->X, vector->Y, vector->Z, 1);
    return csl::math::Vector3(v.x(), v.y(), v.z());
}