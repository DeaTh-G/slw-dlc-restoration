#pragma once

namespace csl
{
    namespace math
    {
        inline static float Max(float value, float max)
        {
            float result = max;
            if (value >= max)
                result = value;
            return result;
        }

        inline static float Min(float value, float min)
        {
            if (min < value)
                return min;
            
            return value;
        }

        inline static float Lerp(float a1, float a2, float a3)
        {
            return (a3 * (a2 - a1)) + a1;
        }

        inline static float Select(float a1, float a2, float a3)
        {
            if (a1 >= 0)
                return a2;
            else
                return a3;
        }
    }
}