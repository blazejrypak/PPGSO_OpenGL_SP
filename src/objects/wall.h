#pragma once

#ifndef FINALFINAL_WALL_H
#define FINALFINAL_WALL_H

#include <bin/ppgso/lib/ppgso.h>
#include "../objects/object.h"

using namespace ppgso;
using namespace glm;

class Wall : public Object {

private:

    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;

public:

    glm::vec3 color = {0, 0, 0};

    Wall ();
    Wall (const std::string& textureName, const std::string& objectName);

    bool update (Scene &scene, float dt) override;

    void render (Scene &scene) override;
};


#endif //FINALFINAL_WALL_H
