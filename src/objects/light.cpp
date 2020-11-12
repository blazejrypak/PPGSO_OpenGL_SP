#include "light.h"
#include "../common/scene.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

Light::Light () {
    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = make_unique<Mesh>("objects/cube.obj");

}

bool Light::update (Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Light::render (Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("OverallColor", this->color);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewPosition", scene.camera->current.position);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    mesh->render();
}
