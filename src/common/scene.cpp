#include "scene.h"

void Scene::init() {
    this->objects.clear();
//    this->lightReset();
}

void Scene::update(float time) {

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);

    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(*this, time))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else{
            if (keyframe_animation){
                this->keyframeAnimationDeltaTime = time - this->keyframeAnimationStartDeltaTime;
                if (this->keyframeAnimationDeltaTime >= this->keyframeAnimationDuration) {
                    this->keyframe_animation = false;
                    continue;
                }
                obj->update_keyframe(*this, time);
            }
            ++i;
        }
    }
}

void Scene::render() {
    // Simply render all objects
    for (auto &obj : objects) {
        obj->render(*this);
    }
}

std::vector<Object *> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
    std::vector<Object *> intersected = {};
    for (auto &object : objects) {
        // Collision with sphere of size object->scale.x
        auto oc = position - object->position;
        auto radius = object->scale.x;
        auto a = glm::dot(direction, direction);
        auto b = glm::dot(oc, direction);
        auto c = glm::dot(oc, oc) - radius * radius;
        auto dis = b * b - a * c;

        if (dis > 0) {
            auto e = sqrt(dis);
            auto t = (-b - e) / a;

            if (t > 0) {
                intersected.push_back(object.get());
                continue;
            }

            t = (-b + e) / a;

            if (t > 0) {
                intersected.push_back(object.get());
                continue;
            }
        }
    }
    return intersected;
}

void Scene::lightReset() {
    this->lightDirection = vec3{-1.0f, 5.0f, -1.0f};
    this->lightColor = vec3{1.0f, 1.0f, 1.0f};
}

void Scene::handleKey(int key, int action) {}

Object *Scene::getObject(std::string _ID) {
    for (auto &object : objects) {
        if (object->ID == _ID) {
            return object.get();
        }
    }
    return nullptr;
}


// https://stackoverflow.com/questions/27751602/interpolation-between-2-4x4-matrices
glm::mat4 Scene::interpolate(const mat4 &_mat1, const mat4 &_mat2, const float t) {
    glm::quat rot0 = glm::quat_cast(_mat1);
    glm::quat rot1 = glm::quat_cast(_mat2);

    glm::quat finalRot = glm::slerp(rot0, rot1, t);

    glm::mat4 finalMat = glm::mat4_cast(finalRot);

    finalMat[3] = _mat1[3] * (1 - t) + _mat2[3] * t;

    return finalMat;
}

glm::mat4 Scene::getAnimationViewFrame(const mat4 &_mat1, const mat4 &_mat2, const mat4 &_mat3, const mat4 &_mat4,
                                        const float t) {
    glm::mat4 a = this->interpolate(_mat1, _mat2, t);
    glm::mat4 b = this->interpolate(_mat2, _mat3, t);
    glm::mat4 c = this->interpolate(_mat3, _mat4, t);

    glm::mat4 d = this->interpolate(a, b, t);
    glm::mat4 e = this->interpolate(b, c, t);

    return this->interpolate(d, e, t);
}