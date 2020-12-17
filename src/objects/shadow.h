#pragma once

#ifndef FINALFINAL_SHADOW_H
#define FINALFINAL_SHADOW_H

#include <bin/ppgso/lib/ppgso.h>
#include "../objects/object.h"

using namespace ppgso;
using namespace glm;

class Shadow : public Object {

private:

    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:

    Shadow(vec3 position, vec3 scale);

    void update(vec3 position, Scene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

    void update_keyframe(Scene &scene, float dt) override;
};


#endif //FINALFINAL_SHADOW_H
