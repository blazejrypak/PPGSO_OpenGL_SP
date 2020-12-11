//
// Created by Blažej Rypák on 20/11/2020.
//

#pragma once

#include <src/objects/shape.h>
#include "scene.h"

#ifndef FINALFINAL_MENU_SCENE_H
#define FINALFINAL_MENU_SCENE_H

class MenuScene final : public Scene {

private:

public:
    bool animate = false;

    Shape *cone_1 = nullptr;
    Shape *cone_2 = nullptr;
    Shape *cone_3 = nullptr;

    void init();

    void update(float time) override;

    void handleKey(int key, int action) override;
};

#endif //FINALFINAL_MENU_SCENE_H
