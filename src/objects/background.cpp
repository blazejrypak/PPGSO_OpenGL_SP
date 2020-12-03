#include "background.h"

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

Background::Background() {
  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
  if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/skybox/top.bmp"));
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("objects/cube.obj");
}

bool Background::update(Scene &scene, float dt) {
  // Offset for UV mapping, creates illusion of scrolling
//  textureOffset.x -= dt/2;

  generateModelMatrix();
  return true;
}

void Background::render(Scene &scene) {
  // Disable writing to the depth buffer so we render a "background"
  glDepthMask(GL_FALSE);

  // NOTE: this object does not use camera, just renders the entire quad as is
  shader->use();

  // Pass UV mapping offset to the shader
  shader->setUniform("TextureOffset", textureOffset);

  // Render mesh, not using any projections, we just render in 2D
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("ViewMatrix", glm::mat4{1.0f});
  shader->setUniform("ProjectionMatrix", glm::mat4{1.0f});
  shader->setUniform("Texture", *texture);
  mesh->render();

  glDepthMask(GL_TRUE);
}

// shared resources
std::unique_ptr<ppgso::Mesh> Background::mesh;
std::unique_ptr<ppgso::Shader> Background::shader;
std::unique_ptr<ppgso::Texture> Background::texture;
