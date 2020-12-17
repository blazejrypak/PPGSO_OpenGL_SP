#include "shadow.h"
#include "../common/scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

unique_ptr<Mesh> Shadow::mesh;
unique_ptr<Texture> Shadow::texture;
unique_ptr<Shader> Shadow::shader;

Shadow::Shadow(vec3 position, vec3 scale) {
    this->position = vec3{position.x, 1.0f, position.z};
    this->scale = vec3{scale.x, 0.1f, scale.z};

    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture)
        texture = make_unique<Texture>(image::loadBMP("textures/blacc.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("objects/sphere.obj");

}

bool Shadow::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Shadow::update(vec3 position, Scene &scene) {
    this->position = vec3{position.x - (scene.sun->dirLight.direction.x / 1.5), 0.5f, position.z};
    this->scale.x = abs(scene.sun->dirLight.direction.x) + scale.x / 2;
}

void Shadow::render(Scene &scene) {
    shader->use();

    shader->setUniform("Transparency", .4f);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    // Disable depth testing
    glDisable(GL_DEPTH_TEST);

    // Enable blending
    glEnable(GL_BLEND);

    // Additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    mesh->render();

    // Disable blending
    glDisable(GL_BLEND);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    mesh->render();
}

void Shadow::update_keyframe(Scene &scene, float dt) {}