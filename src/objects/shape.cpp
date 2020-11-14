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

    if (this->_type == "out"){
    shader->setUniform("dirLight.direction", scene.sun->dirLight.direction);
    shader->setUniform("dirLight.ambient", scene.sun->dirLight.ambient);
    shader->setUniform("dirLight.diffuse", scene.sun->dirLight.diffuse);
    shader->setUniform("dirLight.specular", scene.sun->dirLight.specular);
    }

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

    shader->setUniform( "spotLightOn", scene.flash_light_on);
    if (scene.flash_light_on){
        shader->setUniform( "spotLight.position", scene.light->spotLight.position);
        shader->setUniform( "spotLight.direction", scene.light->spotLight.direction);
        shader->setUniform( "spotLight.ambient", scene.light->spotLight.ambient);
        shader->setUniform( "spotLight.diffuse", scene.light->spotLight.diffuse);
        shader->setUniform( "spotLight.specular", scene.light->spotLight.specular);
        shader->setUniform( "spotLight.constant", scene.light->spotLight.constant);
        shader->setUniform( "spotLight.linear", scene.light->spotLight.linear);
        shader->setUniform( "spotLight.quadratic", scene.light->spotLight.quadratic);
        shader->setUniform( "spotLight.cutOff", scene.light->spotLight.cutOff);
        shader->setUniform( "spotLight.outerCutOff", scene.light->spotLight.outerCutOff);
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

void Shape::setPhongLight(bool phongLight) {
    phong_light = phongLight;
}


