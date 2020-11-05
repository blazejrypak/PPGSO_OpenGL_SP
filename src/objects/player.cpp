//
// Created by František Gič on 30/11/2019.
//

#include <thread>
#include <chrono>
#include "../common/scene.h"
#include "../common/main_window.h"
#include "player.h"

using namespace std;
using namespace glm;
using namespace ppgso;

// Static resources
unique_ptr<Mesh> Player::mesh;
unique_ptr<Texture> Player::texture;
unique_ptr<Shader> Player::shader;

Player::Player () {
    this->shadow = new Shadow(this->position+glm::vec3{0.f, 10.f, 0.f}, {this->scale.x*5, 0.1, this->scale.z*5});
    this->shadow->rotation.z = glm::radians(90.f);
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture)
        texture = make_unique<Texture>(image::loadBMP("textures/cyborg.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("objects/cyborg.obj");
}

bool Player::update (Scene &scene, float dt) {
    shadow->update(this->position, scene);
    shadow->update(scene, dt);
    shadow->render(scene);

    generateModelMatrix();
    return true;
}

void Player::render (Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    mesh->render();
}


void Player::handleKey (Scene &scene, int key){
    float speed = 0.5f;
//    W -> 0
//    D -> 1
//    S -> 2
//    A -> 3
    if (key == GLFW_KEY_W){
        if (direction == 0 || abs(direction) == 2){
            if (direction == 0){
                position.z += speed;
            } else {
                position.z -= speed;
            }
        } else {
            if (direction == 1 || direction == -3) {
                position.x -= speed;
            } else {
                position.x += speed;
            }
        }
    } else if (key == GLFW_KEY_D && scene.keyboard[key] == GLFW_PRESS){
        direction += 1;
    } else if (key == GLFW_KEY_A && scene.keyboard[key] == GLFW_PRESS){
        direction -= 1;
    } else if (key == GLFW_KEY_S && scene.keyboard[key] == GLFW_PRESS) {
        direction += 2;
    }
    direction %= 4;
    rotation.z = glm::radians(static_cast<float>(direction) * -90.0f);
    if (scene.camera->isFirstPersonMode()){
        int d = (direction+2)%4;
        d = d == -1 ? 3 : d;
        scene.camera->updateWithDirection(position, abs(static_cast<short>(d)));
    }
}


