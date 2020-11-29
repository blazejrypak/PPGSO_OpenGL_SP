#include <thread>
#include <chrono>
#include "day_scene.h"
#include "main_window.h"
#include "../objects/wall.h"
#include "../objects/shape.h"
#include "../objects/weather.h"
#include "../objects/common.h"
#include "../objects/light.h"


void DayScene::init() {
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

    auto light_1 = std::make_unique<Light>();
    light_1->position = {23.5f, 8.f, 0.75f};
    light_1->color = {0.5, 0.f, 0.f};
    light_1->scale = {0.25f, 0.25f, 0.25f};
    light_1->pointLight.position = light_1->position;
    light_1->pointLight.constant = 1.0f;
    light_1->pointLight.linear = 0.09f;
    light_1->pointLight.quadratic = 0.032f;
    light_1->pointLight.ambient = {.5f, 0.5f, 0.5f};
    light_1->pointLight.diffuse = {.5f, .5f, .5f};
    light_1->pointLight.specular = {1.0f, 1.0f,1.0f};
    this->lights.push_back(light_1.get());
    this->objects.push_back(move(light_1));

    auto light_2 = std::make_unique<Light>();
    light_2->position = {19.5f, 8.f, 0.75f};
    light_2->color = {0.5, 0.0, 0.0};
    light_2->scale = {0.25f, 0.25f, 0.25f};
    light_2->pointLight.position = light_2->position;
    light_2->pointLight.constant = 1.0f;
    light_2->pointLight.linear = 0.09f;
    light_2->pointLight.quadratic = 0.032f;
    light_2->pointLight.ambient = {.5f, 0.5f, 0.5f};
    light_2->pointLight.diffuse = {.5f, .5f, .5f};
    light_2->pointLight.specular = {1.0f, 1.0f,1.0f};
    this->lights.push_back(light_2.get());
    this->objects.push_back(move(light_2));

    auto flash_light = std::make_unique<Light>();
    flash_light->spotLight.constant = 1.0f;
    flash_light->spotLight.linear = 0.007f;
    flash_light->spotLight.quadratic = 0.0002f;
    flash_light->spotLight.ambient = {1.f, 0.f, 0.f};
    flash_light->spotLight.diffuse = {.8f, .8f, .8f};
    flash_light->spotLight.specular = {1.0f, 1.0f,1.0f};
    flash_light->spotLight.position = {0.0f, 15.0f,0.0f};
    flash_light->spotLight.direction = {0.0f, -1.f, 0.f};
    flash_light->spotLight.cutOff = glm::cos(glm::radians(6.5f));
    flash_light->spotLight.outerCutOff = glm::cos(glm::radians(7.5f));
    this->light = flash_light.get();

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
    basement->_type = "out";
    this->objects.push_back(move(basement));

    auto road = std::make_unique<Shape>("road", "road/road");
    road->position = {15.f, 0.f, -35.f};
    road->scale = {3.f, 3.f, 30.f};
    road->_type = "out";
    road->rotation = {0.f, 0.f, glm::radians(90.f)};
    this->objects.push_back(move(road));

    auto car = std::make_unique<Shape>("cars/car_1", "cars/car_1");
    car->position = {15.f, 0.5f, -35.f};
    car->_type = "out";
    this->car_ = car.get();
    this->objects.push_back(move(car));

    for (int i = -5; i < 5; ++i) {
        auto lamp_post = std::make_unique<Shape>("white", "lamp_post/lamp_post");
        lamp_post->position = {15.f*i, 0.f, -40.f};
        lamp_post->scale = {0.3f, 0.3f, 0.3f};
        lamp_post->_type = "out";
        lamp_post->rotation = {0.f, 0.f, glm::radians(-90.f)};
        this->objects.push_back(move(lamp_post));
    }

    auto player_ = std::make_unique<Player>();
    player_->position = {2.f,0.5f,-20.f };
    player_->scale = {1.f, 1.f, 1.f};
    player_->minXYZ = {2.f+1.5f,0.5f,-20.f - 0.5f};
    player_->maxXYZ = {2.f-1.5f,0.5f,-20.f + 0.5f};
    player_->_type = "out";
    this->player = player_.get();
    this->objects.push_back(move(player_));

    struct house_obj {
        std::string texturePath;
        std::string objPath;
        std::string type;
        std::string ID;
    };

    std::vector<house_obj> house_objects = {
            {"house/BanheiraTexture", "house/Bathtub", "base", ""},
            {"house/TetoTexture", "house/Bed", "base", ""},
            {"house/CasaTexture", "house/Casa", "out", ""},
            {"house/ChaoTexture", "house/Chao", "out", ""},
            {"house/PortaTexture", "house/door1", "out", "door"},
//            {"house/PortaTexture", "house/Porta1", "door", ""},
//            {"house/PortaTexture", "house/Porta2", "door", ""},
            {"house/TetoTexture", "house/Sink", "base", ""},
            {"house/TetoTexture", "house/Table", "base", ""},
            {"house/TetoTexture", "house/Teto", "base", ""},
            {"house/TetoTexture", "house/Window", "base", ""},
            {"house/TetoTexture", "house/Window1", "base", ""},
            {"house/TetoTexture", "house/Window2", "base", ""},
            {"house/TetoTexture", "house/Window3", "base", ""},
            {"house/VentiladorTexture", "house/Ventilator", "base", ""},
            {"house/LuminariaTexture", "house/Luminaria", "base", ""},
            {"house/LuminariaTexture", "house/Luminaria1", "base", ""},
            {"house/LuminariaTexture", "house/Luminaria2", "base", ""},
            {"house/BanheiraTexture", "home/roof", "out", "roof"},
    };

    glm::vec3 house_base_position_objects = glm::vec3{5.f, 8.f, 0.f};
    glm::vec3 house_base_scale_objects = glm::vec3{5.f, 5.f, 5.f};
    glm::vec3 house_base_rotation_objects = glm::vec3{0.f, 0.f, glm::radians(180.f)};

    for (auto & house_object : house_objects) {
        auto obj = std::make_unique<Shape>(house_object.texturePath, house_object.objPath);
        if (house_object.ID == "door"){
            obj->position = { -2.f,  6.f, -8.f};
            obj->scale = house_base_scale_objects;
            obj->_type = house_object.type;
            obj->ID = house_object.ID;
        } else if(house_object.ID == "roof"){
            obj->ID = house_object.ID;
            obj->position = {5.f, 0.f, 0.f};
            obj->scale = {8.0f, 5.0f, 5.f};
        }
        else {
            obj->ID = house_object.ID;
        obj->position = house_base_position_objects;
        obj->scale = house_base_scale_objects;
        obj->rotation = house_base_rotation_objects;
        obj->_type = house_object.type;
        }
        this->objects.push_back(move(obj));
    }

    // WEATHER
    auto weather = std::make_unique<Weather>(this);
    this->_weather = move(weather);

    auto pool = std::make_unique<Shape>("house/BanheiraTexture", "pool/pool1");
    pool->position = {15.f, 1.f, -18.f};
    pool->scale = {5.f, 5.f, 5.f};
    pool->_type = "out";
    this->_weather->_pool = pool.get();
    this->objects.push_back(move(pool));

    auto pool_volume = std::make_unique<Shape>("blau", "pool/pool_volume");
    pool_volume->position = {15.f, 1.f, -18.f};
    pool_volume->scale = {5.f, 0.1f, 5.f};
    pool_volume->_type = "out";
    this->_weather->_pool_volume = pool_volume.get();
    pool_volume->minXYZ = {15.f+7.5f, 1.f, -18.f-5.f};
    pool_volume->maxXYZ = {15.f-7.5f, 1.f, -18.f+5.f};
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
    float day_length = 30.f;

    if (this->car_->position.x < -100.f){
        this->car_->position.x = 100.f;
    }
    this->car_->position.x -= day_length/60.f;



    float sunShinness = fmod(time, day_length);
    float interpolation = 0.0f;
    if (sunShinness < day_length/2){
        interpolation = sunShinness*(2*0.8f/day_length);
    } else {
        interpolation = (day_length - sunShinness)*(2*0.8f/day_length);
    }
    this->sun->dirLight.ambient = vec3(interpolation, interpolation, interpolation) * this->sun->color;
    this->sun->dirLight.direction = {interpolation, interpolation, interpolation};
    Scene::update(time);
}

void DayScene::handleKey (int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {

            //Exit to the menu
            case (GLFW_KEY_Q): {
                this->windowRef->openMenu();
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
                // Pause raining
            case (GLFW_KEY_J): {
                this->_weather->upWindSpeed();
                break;
            }
                // Pause raining
            case (GLFW_KEY_K): {
                this->_weather->downWindSpeed();
                break;
            }
                // Change scene light color
            case (GLFW_KEY_I): {
                float r = glm::linearRand(0.0f, 1.0f);
                float g = glm::linearRand(0.0f, 1.0f);
                float b = glm::linearRand(0.0f, 1.0f);
                this->sun->color = {r, g, b};
                break;
            }
            default:
                break;
        }
    }

    else if (key == GLFW_KEY_B) {
        this->sun->dirLight.direction.x -= 0.2f;
        this->sun->position.x -= 0.2f;
        if (this->sun->dirLight.ambient.x < 0.8){
        this->sun->dirLight.ambient.x += 0.05f;
        this->sun->dirLight.ambient.y += 0.05f;
        this->sun->dirLight.ambient.z += 0.05f;
        }
    }
    else if (key == GLFW_KEY_N) {
        this->sun->dirLight.direction.x += 0.2f;
        this->sun->position.x += 0.2f;
        if (this->sun->dirLight.ambient.x > 0.05){
            this->sun->dirLight.ambient.x -= 0.05f;
            this->sun->dirLight.ambient.y -= 0.05f;
            this->sun->dirLight.ambient.z -= 0.05f;
        }

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