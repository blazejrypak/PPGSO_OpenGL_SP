#include <glm/glm.hpp>

#include "camera.h"
#include "day_scene.h"

using namespace std;
using namespace glm;
using namespace ppgso;


Camera::Camera(float fow, float ratio, float near, float far) {
    float fowInRad = (PI / 180.0f) * fow;
    projectionMatrix = perspective(fowInRad, ratio, near, far + 1000.f);
    worldUp = this->views[this->view].up;
    this->update(this->views[this->view]);
    this->resetAnimation();
}


void Camera::update() {
    this->update(current.position, current.center);
}

void Camera::updatePosition(vec3 position) {
    this->update(position, current.center);
}

void Camera::update(vec3 eye, vec3 center) {
    viewMatrix = lookAt(eye, center + front, globalUp);
}

void Camera::update(View view) {
    current = view;
    this->update();
}

void Camera::update(vec3 center) {
    viewMatrix = lookAt(current.position, center + front, globalUp);
}

void Camera::updateWithDirection(vec3 position, short direction) {
    firstPersonViewCenter = position;
    current.position = position + this->getDirectionMatrix(direction);
    current.center = firstPersonViewCenter;
    this->update();
}

vec3 Camera::getDirectionMatrix(short direction) {
    int y = 5;
    int x = 6;
    int z = 10;
    switch (direction) {
        case 0: {
            firstPersonViewCenter.z -= z;
            return vec3{0, y, x};
        }
        case 3: {
            firstPersonViewCenter.x -= z;
            return vec3{x, y, 0};
        }
        case 1: {
            firstPersonViewCenter.x += z;
            return vec3{-x, y, 0};
        }
        case 2: {
            firstPersonViewCenter.z += z;
            return vec3{0, y, -x};
        }
    }
}

void Camera::switchView(DayScene *scene) {
    view++;
    if (view == views.size()) {
        view = 0;
    };
    if (view == 1) {
        views[view].position = scene->player->position + this->getDirectionMatrix(scene->player->direction);

        views[view].center = scene->player->position;
    }
    this->update(views[view]);
}

void Camera::handleKey(Scene &scene, int key) {

    if (view != 1) {
        float speed = 0.2f;
        float speedRotation = 2.5f;

        if (key == GLFW_KEY_X && scene.keyboard[key] == GLFW_PRESS) {
//            animationStartDeltaTime = scene.deltaTime;
//            animationRunning = true;
            scene.keyframeAnimationStartDeltaTime = scene.deltaTime;
            scene.keyframe_animation = true;
        }

        if (key == GLFW_KEY_O) {
            current.position -= (current.center - current.position) * (speed / 2);
        }
        if (key == GLFW_KEY_P) {
            current.position += (current.center - current.position) * (speed / 2);
        }

        if (key == GLFW_KEY_UP) {
            current.center.y += speedRotation;
        }

        if (key == GLFW_KEY_DOWN) {
            current.center.y -= speedRotation;
        }

        if (key == GLFW_KEY_LEFT) {
            current.center.x += speedRotation;
        }

        if (key == GLFW_KEY_RIGHT) {
            current.center.x -= speedRotation;
        }
        this->update();
    }
};

bool Camera::isFirstPersonMode() {
    return view == 1;
}

View Camera::getView() {
    return this->current;
}

void Camera::resetAnimation() {
    animationFrames.clear();
//    animationRunning = false;
//    animationDeltaTime = 0.0f;
}

// https://stackoverflow.com/questions/27751602/interpolation-between-2-4x4-matrices
glm::mat4 Camera::interpolate(const mat4 &_mat1, const mat4 &_mat2, const float t) {
    glm::quat rot0 = glm::quat_cast(_mat1);
    glm::quat rot1 = glm::quat_cast(_mat2);

    glm::quat finalRot = glm::slerp(rot0, rot1, t);

    glm::mat4 finalMat = glm::mat4_cast(finalRot);

    finalMat[3] = _mat1[3] * (1 - t) + _mat2[3] * t;

    return finalMat;
}

glm::mat4 Camera::getAnimationViewFrame(const mat4 &_mat1, const mat4 &_mat2, const mat4 &_mat3, const mat4 &_mat4,
                                        const float t) {
    glm::mat4 a = this->interpolate(_mat1, _mat2, t);
    glm::mat4 b = this->interpolate(_mat2, _mat3, t);
    glm::mat4 c = this->interpolate(_mat3, _mat4, t);

    glm::mat4 d = this->interpolate(a, b, t);
    glm::mat4 e = this->interpolate(b, c, t);

    return this->interpolate(d, e, t);
}