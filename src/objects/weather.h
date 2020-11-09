#pragma once

#ifndef FINALFINAL_WEATHER_H
#define FINALFINAL_WEATHER_H

#include <bin/ppgso/lib/ppgso.h>
#include "../objects/object.h"
#include "../objects/shape.h"
#include "../common/scene.h"

using namespace ppgso;
using namespace glm;

class Weather {

private:
    bool raining = false;
    bool pause = false;

    class RainParticle final : public Object {

        static std::unique_ptr<ppgso::Mesh> mesh;
        static std::unique_ptr<ppgso::Shader> shader;
        static std::unique_ptr<ppgso::Texture> texture;

    private:
        Weather *parent;
        vec3 speed = {0, -1.0f, 0};
        float age{0.0f};
        float maxAge{2.f};

    public:
        RainParticle( vec3 position, Weather *parent);
        bool update(Scene &scene, float dt) override;
        void render(Scene &scene) override;
    };

public:
    vec3 windSpeed = {0, 0, 0};
    Scene *scene;
    Shape *_pool;
    Shape *_pool_volume;

    explicit Weather (Scene *scene);
    void update ();
    void toggleRain();
    void pauseRain();
};


#endif //FINALFINAL_WEATHER_H