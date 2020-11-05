#include <random>
#include "weather.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

using namespace std;

unique_ptr<Shader> Weather::RainParticle::shader;
unique_ptr<Mesh> Weather::RainParticle::mesh;
unique_ptr<Texture> Weather::RainParticle::texture;

Weather::RainParticle::RainParticle(vec3 position, Weather *parent) {
    this->position = position;
    this->scale = vec3{.5f, 1.5f, 0.5f};
    this->parent = parent;

    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("textures/blau.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("objects/sphere.obj");
}

bool Weather::RainParticle::update(Scene &scene1, float dt) {
    if (this->parent->pause) {
        return true;
    }
    if (this->age >= this->maxAge) {
        return false;
    }
    if (this->position.y > 1) {
        position += vec3{(speed.x * dt) + this->parent->windSpeed.x, speed.y * dt, speed.z * dt};
        speed += speed * dt;
        generateModelMatrix();
        return true;
    }
    if (position.y <= 1) {
        if (position.x > this->parent->_pool_volume->position.x - 5 ||
            position.x < this->parent->_pool_volume->position.x + 5 ||
            position.z > this->parent->_pool_volume->position.z - 5 ||
            position.z < this->parent->_pool_volume->position.z + 5) {
            if (this->parent->_pool_volume->scale.y < this->parent->_pool->scale.y){
                this->parent->_pool_volume->scale.y += 0.00001f;
                this->age += 0.2f;
            }
        } else {
            scale -= vec3{0.1, 0.2, 0.1};
            this->age += 0.5f;
//            return false;
        }
    }
    generateModelMatrix();
    return true;
}

void Weather::RainParticle::render(Scene &scene) {
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

Weather::Weather(Scene *scene) {
    this->scene = scene;
}

void Weather::toggleRain() {
    this->raining = !this->raining;
}

void Weather::pauseRain() {
    this->pause = !this->pause;
}

void Weather::update() {
    if (this->raining && !this->pause) {
        for (int i = 0; i < 100; ++i) {
            vec3 pos = vec3{(rand() % 100)-50, (rand() % 100) + 20, (rand() % 100)-50};
            auto rainParticle = std::make_unique<Weather::RainParticle>(pos, this);
            scene->objects.push_back(move(rainParticle));
        }
    }
}