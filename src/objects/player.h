#pragma once

#ifndef FINALFINAL_PLAYER_H
#define FINALFINAL_PLAYER_H

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include "object.h"
#include "block.h"
#include "shadow.h"

class Player final : public Object {
private:

    bool hasMoved = false;
    float delay = 0.0f;

    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;


    Shadow *shadow;

public:
    vector<int> controls = {
            GLFW_KEY_W,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_A,
    };

    short direction = 0;

    Player ();

    bool update (Scene &scene, float dt) override;

    void render (Scene &scene) override;

    void handleKey (Scene &scene, int key);
};

#endif //FINALFINAL_PLAYER_H



