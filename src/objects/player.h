#pragma once

#ifndef FINALFINAL_PLAYER_H
#define FINALFINAL_PLAYER_H

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

#include "object.h"
#include "block.h"
#include "shadow.h"
#include "shape.h"
#include "light.h"

class Player final : public Object {
private:

    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    bool dead = false;
    float collisionStart = 0.0f;
    float maxDeadAge = 5.0f;


    Shadow *shadow;

    std::vector<glm::mat4> animationKeyFrames = {
//            getModelMatrix({this->position.x + 20.f, this->position.y + 20.f, this->position.z  + 20.f}, {this->rotation.x , this->rotation.y, this->rotation.z},this->scale*2.f),
//            getModelMatrix({this->position.x + 20.f, this->position.y + 20.f, this->position.z + 20.f}, {this->rotation.x , this->rotation.y, this->rotation.z + glm::radians(20.f)},this->scale*1.5f),
//            getModelMatrix({this->position.x + 20.f, this->position.y + 20.f, this->position.z + 20.f}, {this->rotation.x , this->rotation.y, this->rotation.z + glm::radians(70.f)},this->scale*2.f),
//            getModelMatrix({this->position.x + 20.f, this->position.y + 20.f, this->position.z + 20.f}, {this->rotation.x , this->rotation.y, this->rotation.z + glm::radians(70.f)},this->scale*2.f),
    };

public:
    Shape *_pool;
    vector<int> controls = {
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_A,
            GLFW_KEY_E,
            GLFW_KEY_Q,
    };

    short direction = 0;

    bool updated = false;

    Player();

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void handleKey(Scene &scene, int key);

    Shape *checkMove(Scene &scene);

    bool checkPlayerMove(vec3 &nextPosition, Scene &scene);

    void update_keyframe(Scene &scene, float dt) override;
};

#endif //FINALFINAL_PLAYER_H



