#include "shape.h"
#include "../common/scene.h"
#include "shadow.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

Shape::Shape (const std::string& textureName, const std::string& objectName) {
    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
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

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
    shader->setUniform("LightColor", scene.lightColor);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewPosition", scene.camera->current.position);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    mesh->render();
}


