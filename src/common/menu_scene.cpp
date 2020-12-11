//
// Created by Blažej Rypák on 20/11/2020.
//

#include "menu_scene.h"

#include <thread>
#include <chrono>
#include <src/objects/background.h>
#include "main_window.h"
#include "../objects/wall.h"
#include "../objects/shape.h"
#include "../objects/weather.h"
#include "../objects/common.h"
#include "../objects/light.h"


void MenuScene::init() {
    Scene::init();

    // Add background
    this->objects.push_back(std::make_unique<Background>());

    auto sun = std::make_unique<Light>();
    sun->position = {0.f, 100.f, 0.f};
    sun->color = {1.f, 1.f, 1.f};
    sun->scale = {1.f, 1.f, 1.f};
    sun->dirLight.direction = {0.f, -1.f, 0.f};
    sun->dirLight.ambient = {0.2f, 0.2f, 0.2f};
    sun->dirLight.diffuse = {.5f, .5f, .5f};
    sun->dirLight.specular = {1.0f, 1.0f, 1.0f};
    this->sun = sun.get();
    this->objects.push_back(move(sun));

    auto cone = std::make_unique<Shape>("grass", "menu/cone");
    cone->position = {0, 15, 0};
    cone->scale = {5.f, 5.f, 5.f};
    cone->_type = "out";
    cone_1 = cone.get();
    this->objects.push_back(move(cone));

    auto cone2 = std::make_unique<Shape>("blau", "menu/cone");
    cone2->position = {0, 25, 0};
    cone2->scale = {7.f, 7.f, 7.f};
    cone2->_type = "out";
    cone_2 = cone2.get();
    cone_2->parentModelMatrix = &cone_1->modelMatrix;
    this->objects.push_back(move(cone2));

    auto cone3 = std::make_unique<Shape>("blau", "menu/cone");
    cone3->position = {0, 25, 0};
    cone3->scale = {7.f, 7.f, 7.f};
    cone3->_type = "out";
    cone_3 = cone3.get();
    cone_3->parentModelMatrix = &cone_2->modelMatrix;
    this->objects.push_back(move(cone3));

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
    cone_1->position = {20, 25, 0};
    cone_1->rotation.y += glm::radians(5.f);
    cone_1->update(*this, time);
    cone_2->position = {0, 2, 0};
    cone_2->rotation.y += glm::radians(10.f);
    cone_2->scale = {1.f, 1.f, 1.f};
    cone_2->update(*this, time);
    cone_3->position = {0, 1, 1};
    cone_3->rotation.y += glm::radians(30.f);
    cone_3->scale = {0.5f, 0.5f, 0.5f};
    Scene::update(time);
}

void MenuScene::handleKey(int key, int action) {
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
