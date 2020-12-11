#include "shape.h"
#include "../common/scene.h"
#include "car.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

Car::Car(const std::string &textureName, const std::string &objectName) {
    // Initialize static resources if needed
    if (!shader) {
        shader = make_unique<Shader>(phong_vert_glsl, phong_frag_glsl);
    }
    if (!texture)
        texture = make_unique<Texture>(image::loadBMP("textures/" + textureName + ".bmp"));
    if (!mesh) mesh = make_unique<Mesh>("objects/" + objectName + ".obj");

}

bool Car::haveCollision(Scene &scene) {
    for (int i = 0; i < scene.cars.size(); ++i) {
//            37.5  17.5
//        50 30 25 -5
//        <  <  >  >
        if (scene.cars[i] != this && scene.cars[i]->position.z == this->position.z &&
            static_cast<int>(scene.cars[i]->position.x) == static_cast<int>(this->position.x)) {
            scene.cars.erase(scene.cars.begin() + i);
            return true;
        }
//        if (scene.cars[i] != this && scene.cars[i]->position.z == this->position.z && scene.cars[i]->position.x + scene.cars[i]->scale.x <  this->position.x - this->scale.x){
//            std::cout << scene.cars[i]->position.x + scene.cars[i]->scale.x << " " << this->position.x - this->scale.x << std::endl;
//            scene.cars.erase(scene.cars.begin() + i);
//            return true;
//        }
    }
    return false;
}

bool Car::update(Scene &scene, float dt) {
    if (this->position.x < this->minPos.x) {
        for (size_t i = 0; i < scene.cars.size(); ++i) {
            if (this == scene.cars[i]) {
                scene.cars.erase(scene.cars.begin() + i);
                auto car = std::make_unique<Car>("fire", "cars/LowPolyFiatUNO");
                car->rotation.z = glm::radians(-90.f);
                if (rand() % 3 < 2) {
                    car->position = {50.f, 0.5f, -35.f};
                } else {
                    car->position = {50.f, 0.5f, -30.f};
                }
                car->_type = "out";
                scene.cars.push_back(car.get());
                scene.objects.push_back(move(car));
            }
        }
        return false;
    }
    if (!haveCollision(scene)) {
        this->position += this->direction * dt;
    } else {
        return false;
    }
    generateModelMatrix();
    return true;
}

void Car::render(Scene &scene) {
    shader->use();

    if (this->_type == "out") {
        shader->setUniform("dirLight.direction", scene.sun->dirLight.direction);
        shader->setUniform("dirLight.ambient", scene.sun->dirLight.ambient);
        shader->setUniform("dirLight.diffuse", scene.sun->dirLight.diffuse);
        shader->setUniform("dirLight.specular", scene.sun->dirLight.specular);
    }

    // Set up light
    for (unsigned int i = 0; i < scene.lights.size(); ++i) {
        shader->setUniform("pointLightsOn", true);
        shader->setUniform("pointLights[" + to_string(i) + "].position", scene.lights[i]->position);
        shader->setUniform("pointLights[" + to_string(i) + "].ambient", scene.lights[i]->pointLight.ambient);
        shader->setUniform("pointLights[" + to_string(i) + "].diffuse", scene.lights[i]->pointLight.diffuse);
        shader->setUniform("pointLights[" + to_string(i) + "].specular", scene.lights[i]->pointLight.specular);
        shader->setUniform("pointLights[" + to_string(i) + "].constant", scene.lights[i]->pointLight.constant);
        shader->setUniform("pointLights[" + to_string(i) + "].linear", scene.lights[i]->pointLight.linear);
        shader->setUniform("pointLights[" + to_string(i) + "].quadratic", scene.lights[i]->pointLight.quadratic);
    }

    shader->setUniform("spotLightOn", scene.flash_light_on);
    if (scene.flash_light_on) {
        shader->setUniform("spotLight.position", scene.light->spotLight.position);
        shader->setUniform("spotLight.direction", scene.light->spotLight.direction);
        shader->setUniform("spotLight.ambient", scene.light->spotLight.ambient);
        shader->setUniform("spotLight.diffuse", scene.light->spotLight.diffuse);
        shader->setUniform("spotLight.specular", scene.light->spotLight.specular);
        shader->setUniform("spotLight.constant", scene.light->spotLight.constant);
        shader->setUniform("spotLight.linear", scene.light->spotLight.linear);
        shader->setUniform("spotLight.quadratic", scene.light->spotLight.quadratic);
        shader->setUniform("spotLight.cutOff", scene.light->spotLight.cutOff);
        shader->setUniform("spotLight.outerCutOff", scene.light->spotLight.outerCutOff);
    }


    shader->setUniform("viewPos", scene.camera->getView().position);
    shader->setUniform("material.ambient", material.ambient);
    shader->setUniform("material.diffuse", material.diffuse);
    shader->setUniform("material.specular", material.specular);
    shader->setUniform("material.shininess", material.shininess);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewPosition", scene.camera->current.position);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    mesh->render();
}

