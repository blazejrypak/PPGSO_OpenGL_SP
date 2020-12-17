#pragma once

#ifndef FINALFINAL_LIGHT_H
#define FINALFINAL_LIGHT_H

#include "../bin/ppgso/lib/ppgso.h"
#include "../objects/object.h"

using namespace ppgso;
using namespace glm;

class Light : public Object {

private:

    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;

public:
    struct {
        vec3 position;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } pointLight;

    struct {
        vec3 position;
        vec3 direction;
        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } spotLight;


    struct {
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    } dirLight;

    glm::vec3 color = {1.0, 1.0, 1.0};

    Light();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void update_keyframe(Scene &scene, float dt) override;
};


#endif //FINALFINAL_LIGHT_H
