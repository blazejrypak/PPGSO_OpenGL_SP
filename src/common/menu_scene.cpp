//
// Created by Blažej Rypák on 20/11/2020.
//

#include "menu_scene.h"

#include <thread>
#include <chrono>
#include "main_window.h"
#include "../objects/wall.h"
#include "../objects/shape.h"
#include "../objects/weather.h"
#include "../objects/common.h"
#include "../objects/light.h"


void MenuScene::init() {
    Scene::init();
    float x = 100.f;
    float y = 0.1f;
    float z = 0.1f;
    auto sun = std::make_unique<Light>();
    sun->position = {0.f, 70.f, 0.f};
    sun->color = {1.f, 1.f, 1.f};
    sun->scale = {1.f, 1.f, 1.f};
    sun->dirLight.direction = {0.f, -1.f, 0.f};
    sun->dirLight.ambient = {0.2f, 0.2f, 0.2f};
    sun->dirLight.diffuse = {.5f, .5f, .5f};
    sun->dirLight.specular = {1.0f, 1.0f,1.0f};
    this->sun = sun.get();
    this->objects.push_back(move(sun));

    auto axis_x = std::make_unique<Wall>();
    axis_x->color = {1, 0, 0};
    axis_x->scale = {x, y, z};
    this->objects.push_back(move(axis_x));

    auto axis_y = std::make_unique<Wall>();
    axis_y->color = {0, 1, 0};
    axis_y->scale = {y, x, z};
    this->objects.push_back(move(axis_y));

    auto axis_z = std::make_unique<Wall>();
    axis_z->color = {0, 0, 1};
    axis_z->scale = {y, z, x};
    this->objects.push_back(move(axis_z));
}

void MenuScene::update(float time) {
    if (this->camera->animationRunning && this->camera->currentAnimationFrameIndex == 0){
        this->camera->animationDeltaTime = time + this->camera->animationFramesPerSecond;
        this->camera->update();
    } else if (this->camera->animationRunning && time > this->camera->animationDeltaTime){
        this->camera->animationDeltaTime += this->camera->animationFramesPerSecond;
        this->camera->update();
    }
    Scene::update(time);
}

void MenuScene::handleKey (int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {

            //Exit to the menu
            case (GLFW_KEY_ESCAPE): {
                this->windowRef->openDayScene();
                break;
            }
            default:
                break;
        }
    }

    // Handle camera
    if (std::find(std::begin(this->camera->controls), std::end(this->camera->controls), key)
        != std::end(this->camera->controls)) {
        this->camera->handleKey(*this, key);
    }
}
