#include "shape.h"
#include "../common/scene.h"
#include "shadow.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

Shape::Shape (const std::string& textureName, const std::string& objectName) {
    // Initialize static resources if needed
    if (!shader){
        shader = make_unique<Shader>(phong_vert_glsl, phong_frag_glsl);
    }
    if (!texture)
        texture = make_unique<Texture>(image::loadBMP("textures/" + textureName + ".bmp"));
    if (!mesh) mesh = make_unique<Mesh>("objects/" + objectName + ".obj");

}

bool Shape::update (Scene &scene, float dt) {
    if (shadow != nullptr){
        shadow->update(this->position, scene);
        shadow->update(scene, dt);
        shadow->render(scene);
    }
    generateModelMatrix();
    return true;
}

void Shape::render (Scene &scene) {
    shader->use();

//    if (dirLight){
//        shader->setUniform("dirLight.direction", dir_light->direction);
//        shader->setUniform("dirLight.ambient", dir_light->ambient);
//        shader->setUniform("dirLight.diffuse", dir_light->diffuse);
//        shader->setUniform("dirLight.specular", dir_light->specular);
//        shader->setUniform("dirLight.specular", dir_light->specular);
//
//        shader->setUniform("viewPos", scene.camera->getView().position);
//        shader->setUniform("material.ambient", material.ambient);
//        shader->setUniform("material.diffuse", material.diffuse);
//        shader->setUniform("material.specular", material.specular);
//        shader->setUniform("material.shininess", material.shininess);
//    }
//    if (pointLight){
//        shader->setUniform("pointLights[0].position", light.position);
//        shader->setUniform("pointLights[0].ambient", light.ambient);
//        shader->setUniform("pointLights[0].diffuse", light.diffuse);
//        shader->setUniform("pointLights[0].specular", light.specular);
//        shader->setUniform("pointLights[0].constant", light.constant);
//        shader->setUniform("pointLights[0].linear", light.linear);
//        shader->setUniform("pointLights[0].quadratic", light.quadratic);
//
//        shader->setUniform("viewPos", scene.camera->getView().position);
//        shader->setUniform("material.ambient", material.ambient);
//        shader->setUniform("material.diffuse", material.diffuse);
//        shader->setUniform("material.specular", material.specular);
//        shader->setUniform("material.shininess", material.shininess);
//
//    }
//    if (spotLight){
//        shader->setUniform("spotLight.position", scene.camera->getView().position);
//        shader->setUniform("spotLight.direction", spot_light->direction);
//        shader->setUniform("spotLight.ambient", spot_light->ambient);
//        shader->setUniform("spotLight.diffuse", spot_light->diffuse);
//        shader->setUniform("spotLight.specular", spot_light->specular);
//        shader->setUniform("spotLight.constant", spot_light->constant);
//        shader->setUniform("spotLight.linear", spot_light->linear);
//        shader->setUniform("spotLight.quadratic", spot_light->quadratic);
//        shader->setUniform("spotLight.cutOff", spot_light->cutOff);
//        shader->setUniform("spotLight.outerCutOff", spot_light->outerCutOff);
//
//        shader->setUniform("viewPos", scene.camera->getView().position);
//        shader->setUniform("material.ambient", material.ambient);
//        shader->setUniform("material.diffuse", material.diffuse);
//        shader->setUniform("material.specular", material.specular);
//        shader->setUniform("material.shininess", material.shininess);
//
//    }

//    if (this->_type == "wall"){
    // Set up light
    for (unsigned int i = 0; i < scene.lights.size(); ++i) {
        shader->setUniform("pointLights[" + to_string(i) + "].position", scene.lights[i]->position);
        shader->setUniform("pointLights[" + to_string(i) + "].ambient", scene.lights[i]->pointLight.ambient);
        shader->setUniform("pointLights[" + to_string(i) + "].diffuse", scene.lights[i]->pointLight.diffuse);
        shader->setUniform("pointLights[" + to_string(i) + "].specular", scene.lights[i]->pointLight.specular);
        shader->setUniform("pointLights[" + to_string(i) + "].constant", scene.lights[i]->pointLight.constant);
        shader->setUniform("pointLights[" + to_string(i) + "].linear", scene.lights[i]->pointLight.linear);
        shader->setUniform("pointLights[" + to_string(i) + "].quadratic", scene.lights[i]->pointLight.quadratic);
    }
//    }

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

void Shape::setPhongLight(bool phongLight) {
    phong_light = phongLight;
}


