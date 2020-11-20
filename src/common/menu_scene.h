//
// Created by Blažej Rypák on 20/11/2020.
//

#pragma once

#include "scene.h"
#ifndef FINALFINAL_MENU_SCENE_H
#define FINALFINAL_MENU_SCENE_H

class MenuScene final : public Scene {

private:

public:
    bool animate = false;

    void init();

    void update (float time) override;

    void handleKey (int key, int action) override;
};

#endif //FINALFINAL_MENU_SCENE_H
