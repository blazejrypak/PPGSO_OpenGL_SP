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

bool checkRoofCollision(Scene &scene, vec3 &particlePos) {
    if ((particlePos.x > scene.roofB->position.x - scene.roofB->scale.x && particlePos.x < scene.roofB->position.x + scene.roofB->scale.x)){
        if (particlePos.z < scene.roofB->scale.z + scene.roofB->position.z && particlePos.z > -scene.roofA->scale.z){
            float roof_base_high = (static_cast<float>(abs(scene.roofA->position.z - scene.roofA->scale.z) /
                                                       std::sqrt(2)));
            auto roof_width = static_cast<float>(abs(2 * scene.roofA->scale.z) / sqrt(2));

            if (particlePos.y <= roof_width - abs(particlePos.z) + roof_base_high){
                return true;
            }
            if (particlePos.y <= scene.roofA->position.y){
                return true;
            }
            return false;
        }
    }
    return false;
}

void Weather::RainParticle::fillPool(){
    if (this->parent->_pool_volume->scale.y < this->parent->_pool->scale.y){
        this->parent->_pool_volume->scale.y += 0.00001f;
    }
}

bool Weather::RainParticle::update(Scene &scene1, float dt) {
    if (this->parent->pause) {
        return true;
    }
    if (this->position.y > 1) {
        this->velocity += (this->gravity + this->parent->windSpeed) * dt;
        this->position += this->velocity;
    }
    // POOL
    if (position.y <= 1) {
        if (position.x > this->parent->_pool_volume->position.x - 5 ||
            position.x < this->parent->_pool_volume->position.x + 5 ||
            position.z > this->parent->_pool_volume->position.z - 5 ||
            position.z < this->parent->_pool_volume->position.z + 5) {
            fillPool();
        }
        else {
            return false;
        }
    }
    if (this->age >= this->maxAge) {
        return false;
    }
    if (this->position.y <= 1){
        return false;
    } else {
        if (checkRoofCollision(scene1, this->position)){
            return false;
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
            vec3 pos = vec3{(rand() % 100)-50, (rand() % 100) + 50, (rand() % 100)-50};
            auto rainParticle = std::make_unique<Weather::RainParticle>(pos, this);
            scene->objects.push_back(move(rainParticle));
        }
    }
}

void Weather::downWindSpeed() {
    this->windSpeed.x -= 0.2f;
    this->windSpeed.y += 0.2f;
}

void Weather::upWindSpeed() {
    this->windSpeed.x += 0.2f;
    this->windSpeed.y -= 0.2f;

}
