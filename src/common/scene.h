#pragma once

#ifndef FINALFINAL_SCENE_H
#define FINALFINAL_SCENE_H

#include <memory>
#include <map>
#include <list>
#include <src/objects/car.h>
#include <src/objects/shape.h>

#include "../objects/object.h"
#include "../objects/light.h"
#include "../common/camera.h"

class SceneWindow;

class Scene {
public:
    SceneWindow *windowRef;

    virtual void update(float time);

    void render();

    void init();

    std::vector<Object *> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    Object *getObject(std::string ID);

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list<std::unique_ptr<Object>> objects;

    // Keyboard state
    std::map<int, int> keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{0.f, 1.f, 1.f};

    glm::vec3 lightColor{1.0f};

    // flashlight
    Light *light;
    Light *sun;
    std::vector<Car *> cars;
    Shape *door;
    Shape *roofA = nullptr;
    Shape *roofB = nullptr;
    float deltaTime;

    bool flash_light_on = false;

    std::vector<Light *> lights;

    virtual void handleKey(int key, int action);

    void lightReset();
};


#endif //FINALFINAL_SCENE_H
