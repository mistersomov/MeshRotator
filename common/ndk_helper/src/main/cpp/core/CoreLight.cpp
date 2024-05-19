#include "CoreLight.hpp"

ndk_helper::core::CoreLight::CoreLight(
    float ambientIntensity,
    glm::vec3 lightColor
) : ambientIntensity{ambientIntensity}, color{lightColor}  {

}
