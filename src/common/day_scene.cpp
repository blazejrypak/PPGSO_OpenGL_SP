#include <thread>
#include <chrono>
#include "day_scene.h"
#include "main_window.h"
#include "../objects/wall.h"
#include "../objects/shape.h"


void DayScene::init() {
    Scene::init();
    float x = 100.f;
    float y = 0.1f;
    float z = 0.1f;
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

    auto sofa = std::make_unique<Shape>("blacc", "sofa/sofa");
    sofa->position = {5.f, -5.f, 0.f};
    sofa->scale = {5.f, 5.f, 5.f};
    this->objects.push_back(move(sofa));

    auto table_circle = std::make_unique<Shape>("blacc", "table_circle/table");
    table_circle->position = {5.f, -5.f, 0.f};
    table_circle->scale = {5.f, 5.f, 5.f};
    this->objects.push_back(move(table_circle));

    auto wall = std::make_unique<Shape>("white", "wall");
    wall->position = {5.f, -5.f, 0.f};
    wall->scale = {5.f, 5.f, 5.f};
    this->objects.push_back(move(wall));

    auto left_wall = std::make_unique<Shape>("white", "left_wall");
    left_wall->position = {5.f, -5.f, 0.f};
    left_wall->scale = {5.f, 5.f, 5.f};
    this->objects.push_back(move(left_wall));

    auto small_table = std::make_unique<Shape>("red", "small_table");
    small_table->position = {5.f, -5.f, 0.f};
    small_table->scale = {5.f, 5.f, 5.f};
    this->objects.push_back(move(small_table));

    auto policka_real = std::make_unique<Shape>("block", "policka_real");
    policka_real->position = {5.f, -5.f, 0.f};
    policka_real->scale = {5.f, 5.f, 5.f};
    this->objects.push_back(move(policka_real));

    auto player_ = std::make_unique<Player>();
    player_->position = {2.f,2.f,2.f };
    player_->scale = {2.f, 2.f, 2.f};
    this->player = player_.get();
    this->objects.push_back(move(player_));

}

void DayScene::update(float time) {
    Scene::update(time);
}

void DayScene::handleKey (int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {

            //Exit to the menu
            case (GLFW_KEY_ESCAPE): {
                this->windowRef->openDayScene();
                break;
            }

                // Restart
            case (GLFW_KEY_R): {
                this->windowRef->startGame();
                break;
            }
                // Pause
            case (GLFW_KEY_L): {
                this->animate = !this->animate;
                break;
            }
                // Switch camera view
            case (GLFW_KEY_C): {
                this->camera->switchView(this);
                break;
            }

            default:
                break;
        }
    }

        // Move the sun ( "Give me an axis and I will move the sun" ~ArchimedesFero )
    else if (key == GLFW_KEY_B) {
        this->lightDirection.x -= 0.4f;
    }
    else if (key == GLFW_KEY_N) {
        this->lightDirection.x += 0.4f;
    }

    // Handle camera
    if (std::find(std::begin(this->camera->controls), std::end(this->camera->controls), key)
        != std::end(this->camera->controls)) {
        this->camera->handleKey(key);
    }

    // Handle player
    if (std::find(std::begin(this->player->controls), std::end(this->player->controls), key)
        != std::end(this->player->controls)) {
        this->player->handleKey(*this, key);
    }
}