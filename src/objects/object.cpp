#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "object.h"

using namespace std;
using namespace glm;

void Object::generateModelMatrix () {
    modelMatrix = glm::translate(mat4(1.0f), position);
    modelMatrix *= glm::orientate4(rotation);
    modelMatrix *= glm::scale(mat4(1.0f), scale);
}