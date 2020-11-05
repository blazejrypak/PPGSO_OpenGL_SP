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
    if (key == GLFW_KEY_W) {
        position.z += speed;
        rotation.z = glm::radians(0.f);
        shadow->rotation.z = glm::radians(90.f);
        direction = 0;
    }
    if (key == GLFW_KEY_S) {
        position.z -= speed;
        rotation.z = glm::radians(-180.f);
        shadow->rotation.z = glm::radians(-90.f);
        direction = 2;
    }
    if (key == GLFW_KEY_D) {
        position.x -= speed;
        rotation.z = glm::radians(-90.f);
        shadow->rotation.z = glm::radians(0.f);
        direction = 3;
    }
    if (key == GLFW_KEY_A) {
        position.x += speed;
        rotation.z = glm::radians(90.f);
        shadow->rotation.z = glm::radians(0.f);
        direction = 1;
    }
    if (scene.camera->isFirstPersonMode()){
        scene.camera->updateWithDirection(position, direction);
    }
}


