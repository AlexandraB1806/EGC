#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Matricea de translatie
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::transpose(
            glm::mat3(1, 0, translateX,
                      0, 1, translateY,
                      0, 0, 1)
        );
    }

    // Matricea de scalare
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(
            glm::mat3(scaleX, 0, 0,
                      0, scaleY, 0,
                      0, 0, 1)
        );
    }

    // Matricea de rotatie
    inline glm::mat3 Rotate(float radians)
    {
        float cosinus = cos(radians);
        float sinus = sin(radians);

        return glm::transpose(
            glm::mat3(cosinus, -sinus, 0,
                      sinus, cosinus, 0,
                      0, 0, 1)
        );
    }
}   // namespace transform2D
