#pragma once

#include "scene.h"

#ifndef FINALFINAL_DAY_SCENE_H
#define FINALFINAL_DAY_SCENE_H

class Player;

class DayScene final : public Scene {

private:
    float _time = 0;

public:
    bool animate = true;

    Player *player;

    void init();

    void update (float time) override;

    void handleKey (int key, int action) override;
};

#endif //FINALFINAL_DAY_SCENE_H