#pragma once

#include "scene.h"
#include "../objects/player.h"
#include "../objects/weather.h"
#include "../objects/shape.h"

#ifndef FINALFINAL_DAY_SCENE_H
#define FINALFINAL_DAY_SCENE_H


class DayScene final : public Scene {

private:

public:
    bool animate = true;

    Player *player;
    unique_ptr<Weather> _weather;

    void init();

    void update (float time) override;

    void handleKey (int key, int action) override;
};

#endif //FINALFINAL_DAY_SCENE_H