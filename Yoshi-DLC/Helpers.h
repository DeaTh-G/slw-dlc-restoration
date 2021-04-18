#pragma once

inline static bool DisablePipeTransition = false;
inline static bool IsConsistentShadow = false;
inline static bool IsAlwaysLinkSonic = true;


static csl::math::Quaternion GetRotationFromMatrix(csl::math::Matrix34* matrix)
{
    Eigen::Matrix3f m;

    for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
            m(i, j) = matrix->data[i][j];
    Eigen::Quaternionf q(m.transpose());

    return csl::math::Quaternion(q.x(), q.y(), q.z(), q.w());
}