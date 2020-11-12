#include <thread>
#include <chrono>
#include "../common/scene.h"
#include "../common/main_window.h"
#include "../objects/shape.h"
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
    if (!shader) shader = make_unique<Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!texture)
        texture = make_unique<Texture>(image::loadBMP("textures/cyborg.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("objects/cyborg.obj");
}

bool Player::update (Scene &scene, float dt) {
    shadow->render(scene);
    shadow->update(this->position, scene);
    shadow->update(scene, dt);
    generateModelMatrix();
//    Shape *obj = checkMove(scene);
//    if (!updated){
//        updated = true;
//        return true;
//    }
//    if (!obj){
//        shadow->update(this->position, scene);
//        shadow->update(scene, dt);
//        generateModelMatrix();
//    } else {
//        if (obj->state){
//            shadow->update(this->position, scene);
//            shadow->update(scene, dt);
////            generateModelMatrix();
//        }
//    }
    return true;
}

void Player::render (Scene &scene) {
    shader->use();

    for (unsigned int i = 0; i < scene.lights.size(); ++i) {
        shader->setUniform("pointLights[" + to_string(i) + "].position", scene.lights[i]->position);
        shader->setUniform("pointLights[" + to_string(i) + "].ambient", scene.lights[i]->pointLight.ambient);
        shader->setUniform("pointLights[" + to_string(i) + "].diffuse", scene.lights[i]->pointLight.diffuse);
        shader->setUniform("pointLights[" + to_string(i) + "].specular", scene.lights[i]->pointLight.specular);
        shader->setUniform("pointLights[" + to_string(i) + "].constant", scene.lights[i]->pointLight.constant);
        shader->setUniform("pointLights[" + to_string(i) + "].linear", scene.lights[i]->pointLight.linear);
        shader->setUniform("pointLights[" + to_string(i) + "].quadratic", scene.lights[i]->pointLight.quadratic);
    }

    shader->setUniform("viewPos", scene.camera->getView().position);
    shader->setUniform("material.ambient", material.ambient);
    shader->setUniform("material.diffuse", material.diffuse);
    shader->setUniform("material.specular", material.specular);
    shader->setUniform("material.shininess", material.shininess);

    // Set up light

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    mesh->render();
}

Shape *Player::checkMove(Scene &scene) {
    auto i = std::begin(scene.objects);

    while (i != std::end(scene.objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        float radius = 0.01f;
        if (obj->_type == "door"){
            if (
                    this->position.x + radius > obj->position.x || this->position.x - radius < obj->position.x ||
                    this->position.y + radius > obj->position.y || this->position.y - radius < obj->position.y ||
                    this->position.z + radius > obj->position.z || this->position.z - radius < obj->position.z
                    ){
                return dynamic_cast<Shape *>(obj);
            }
        }
        i++;
    }
    return nullptr;
}


void Player::handleKey (Scene &scene, int key){
//    if (key == GLFW_KEY_E && scene.keyboard[key] == GLFW_PRESS){
//        Shape *obj = checkMove(scene);
//        std::cout << position.z << " " << obj->position.z << std::endl;
//        if(obj){
//            if (!obj->state){
//                obj->rotation.y = glm::radians(-90.0f);
//                obj->position.z = -15;
//                obj->position.x = 2.5f;
//            } else {
//                obj->rotation.z = glm::radians(180.0f);
//                obj->position.z = 0;
//                obj->position.x = 5.f;
//            }
//            obj->state = !obj->state;
//        }
//        return;
//    }
    float speed = 0.5f;
//    W -> 0
//    D -> 1
//    S -> 2
//    A -> 3
    if (key == GLFW_KEY_W){
        if (direction == 0 || abs(direction) == 2){
            if (direction == 0){
                position.z += speed;
            } else {
                position.z -= speed;
            }
        } else {
            if (direction == 1 || direction == -3) {
                position.x -= speed;
            } else {
                position.x += speed;
            }
        }
    } else if (key == GLFW_KEY_D && scene.keyboard[key] == GLFW_PRESS){
        direction += 1;
    } else if (key == GLFW_KEY_A && scene.keyboard[key] == GLFW_PRESS){
        direction -= 1;
    } else if (key == GLFW_KEY_S && scene.keyboard[key] == GLFW_PRESS) {
        direction += 2;
    }
    direction %= 4;
    rotation.z = glm::radians(static_cast<float>(direction) * -90.0f);
    if (scene.camera->isFirstPersonMode()){
        int d = (direction+2)%4;
        d = d == -1 ? 3 : d;
        scene.camera->updateWithDirection(position, abs(static_cast<short>(d)));
    }
}


