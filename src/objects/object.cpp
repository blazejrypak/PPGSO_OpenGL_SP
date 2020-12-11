#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "object.h"

using namespace std;
using namespace glm;

void Object::generateModelMatrix() {
    if (parentModelMatrix != nullptr) {
        mat4 matrix = *parentModelMatrix;
        matrix *= glm::translate(mat4(1.0f), position);
        matrix *= glm::orientate4(rotation);
        matrix *= glm::scale(mat4(1.0f), scale);
        modelMatrix = matrix;
    } else {
        modelMatrix = glm::translate(mat4(1.0f), position);
        modelMatrix *= glm::orientate4(rotation);
        modelMatrix *= glm::scale(mat4(1.0f), scale);
    }
}

