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

    auto cone = std::make_unique<Shape>("grass", "menu/cone");
    cone->position = {0, 0, 0};
    cone->scale = {50.f, 50.f, 50.f};
    cone->_type = "out";
    this->objects.push_back(move(cone));

    auto desk = std::make_unique<Shape>("grass", "menu/desk");
    desk->position = {0, 7, 0};
    desk->scale = {5.f, 5.f, 5.f};
    desk->_type = "out";
    this->objects.push_back(move(desk));

    auto run_demo_scene = std::make_unique<Shape>("grass", "menu/run_demo_scene");
    run_demo_scene->position = {0, 5, 0};
    run_demo_scene->scale = {5.f, 5.f, 5.f};
    run_demo_scene->rotation = {0, 0, glm::radians(180.f)};
    run_demo_scene->_type = "out";
    this->objects.push_back(move(run_demo_scene));

    auto exit = std::make_unique<Shape>("grass", "menu/exit");
    exit->scale = {5.f, 5.f, 5.f};
    exit->rotation = {0, 0, glm::radians(180.f)};
    exit->_type = "out";
    this->objects.push_back(move(exit));
}

void MenuScene::update(float time) {
    Scene::update(time);
}

void MenuScene::handleKey (int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case (GLFW_KEY_ENTER): {
                this->windowRef->openDayScene();
                break;
            }
                //Exit to the menu
            case (GLFW_KEY_Q): {
                exit(0);
            }
            default:
                break;
        }
    }

    // Handle camera
//    if (std::find(std::begin(this->camera->controls), std::end(this->camera->controls), key)
//        != std::end(this->camera->controls)) {
//        this->camera->handleKey(*this, key);
//    }
}
