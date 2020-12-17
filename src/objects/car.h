#pragma once

#ifndef FINALFINAL_CAR_H
#define FINALFINAL_CAR_H

#include <bin/ppgso/lib/ppgso.h>
#include "object.h"

using namespace ppgso;
using namespace glm;

class Car : public Object {

private:
    vec3 direction = {-linearRand(0.01f, 0.2f), 0, 0};
    vec3 minPos = {-50, 0, 0};

    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

public:

    Car(const std::string &textureName, const std::string &objectName);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    bool haveCollision(Scene &scene);

    void update_keyframe(Scene &scene, float dt) override;
};


#endif //FINALFINAL_CAR_H
