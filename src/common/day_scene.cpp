#include <thread>
#include <chrono>
#include "day_scene.h"
#include "main_window.h"
#include "../objects/wall.h"
#include "../objects/shape.h"
#include "../objects/weather.h"


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

    auto basement = std::make_unique<Shape>("grass", "cube");
    basement->position = {5.f, 0.f, 0.f};
    basement->scale = {50.f, 0.1f, 50.f};
    this->objects.push_back(move(basement));

    auto road = std::make_unique<Shape>("road", "road/road");
    road->position = {15.f, 0.f, -35.f};
    road->scale = {3.f, 3.f, 30.f};
    road->rotation = {0.f, 0.f, glm::radians(90.f)};
    this->objects.push_back(move(road));

    for (int i = -5; i < 5; ++i) {
        auto lamp_post = std::make_unique<Shape>("white", "lamp_post/lamp_post");
        lamp_post->position = {15.f*i, 0.f, -40.f};
        lamp_post->scale = {0.3f, 0.3f, 0.3f};
        lamp_post->rotation = {0.f, 0.f, glm::radians(-90.f)};
        this->objects.push_back(move(lamp_post));

        auto box = std::make_unique<Wall>("red", "sphere");
        box->position = {15.f*i, 3.f, -35.f};
        box->color = {0, 0, 1};
        box->scale = {3.f, 3.f, 3.f};
        this->objects.push_back(move(box));
    }

    auto player_ = std::make_unique<Player>();
    player_->position = {2.f,0.5f,-20.f };
    player_->scale = {1.f, 1.f, 1.f};
    this->player = player_.get();
    this->objects.push_back(move(player_));

    struct house_obj {
        std::string texturePath;
        std::string objPath;
        std::string type;
    };

    std::vector<house_obj> house_objects = {
            {"house/BanheiraTexture", "house/Bathtub", "base"},
            {"house/TetoTexture", "house/Bed", "base"},
            {"house/CasaTexture", "house/Casa", "base"},
            {"house/ChaoTexture", "house/Chao", "base"},
            {"house/PortaTexture", "house/Porta", "door"},
            {"house/PortaTexture", "house/Porta1", "door"},
            {"house/PortaTexture", "house/Porta2", "door"},
            {"house/TetoTexture", "house/Sink", "base"},
            {"house/TetoTexture", "house/Table", "base"},
//            {"house/TetoTexture", "house/Teto", "base"},
            {"house/TetoTexture", "house/Window", "base"},
            {"house/TetoTexture", "house/Window1", "base"},
            {"house/TetoTexture", "house/Window2", "base"},
            {"house/TetoTexture", "house/Window3", "base"},
            {"house/VentiladorTexture", "house/Ventilator", "base"},
            {"house/LuminariaTexture", "house/Luminaria", "base"},
            {"house/LuminariaTexture", "house/Luminaria1", "base"},
            {"house/LuminariaTexture", "house/Luminaria2", "base"},
    };

    glm::vec3 house_base_position_objects = glm::vec3{5.f, 8.f, 0.f};
    glm::vec3 house_base_scale_objects = glm::vec3{5.f, 5.f, 5.f};
    glm::vec3 house_base_rotation_objects = glm::vec3{0.f, 0.f, glm::radians(180.f)};

    for (auto & house_object : house_objects) {
        auto obj = std::make_unique<Shape>(house_object.texturePath, house_object.objPath);
        obj->position = house_base_position_objects;
        obj->scale = house_base_scale_objects;
        obj->rotation = house_base_rotation_objects;
        obj->_type = house_object.type;
        this->objects.push_back(move(obj));
    }

    // WEATHER
    auto weather = std::make_unique<Weather>(this);
    this->_weather = move(weather);

    auto pool = std::make_unique<Shape>("house/BanheiraTexture", "pool/pool1");
    pool->position = {15.f, 1.f, -18.f};
    pool->scale = {5.f, 5.f, 5.f};
    this->_weather->_pool = pool.get();
    this->objects.push_back(move(pool));

    auto pool_volume = std::make_unique<Shape>("blau", "pool/pool_volume");
    pool_volume->position = {15.f, 1.f, -18.f};
    pool_volume->scale = {5.f, 0.1f, 5.f};
    this->_weather->_pool_volume = pool_volume.get();
    this->objects.push_back(move(pool_volume));
}

void DayScene::update(float time) {
    if (this->camera->animationRunning && this->camera->currentAnimationFrameIndex == 0){
        this->camera->animationDeltaTime = time + this->camera->animationFramesPerSecond;
        this->camera->update();
    } else if (this->camera->animationRunning && time > this->camera->animationDeltaTime){
        this->camera->animationDeltaTime += this->camera->animationFramesPerSecond;
        this->camera->update();
    }
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
                // Start raining
            case (GLFW_KEY_G): {
                this->_weather->toggleRain();
                break;
            }
                // Pause raining
            case (GLFW_KEY_H): {
                this->_weather->pauseRain();
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
        this->camera->handleKey(*this, key);
    }

    // Handle player
    if (std::find(std::begin(this->player->controls), std::end(this->player->controls), key)
        != std::end(this->player->controls)) {
        this->player->handleKey(*this, key);
    }
}