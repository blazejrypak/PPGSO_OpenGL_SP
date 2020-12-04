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

    bool hasMoved = false;
    float delay = 0.0f;

    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;


    Shadow *shadow;

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
    bool canMove = false;

    Player ();

    bool update (Scene &scene, float dt) override;

    void render (Scene &scene) override;

    void handleKey (Scene &scene, int key);

    Shape *checkMove(Scene &scene);

    bool checkPlayerMove(vec3 &nextPosition);
};

#endif //FINALFINAL_PLAYER_H



