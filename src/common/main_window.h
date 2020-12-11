#pragma once

#ifndef FINALFINAL_SCENEWINDOW_H
#define FINALFINAL_SCENEWINDOW_H

#include <iostream>
#include <list>
#include <bin/ppgso/lib/ppgso.h>
#include <GL/glew.h>

#include "../common/scene.h"

class Menu;

class Game;

class DayScene;

class MenuScene;

using namespace std;
using namespace glm;
using namespace ppgso;

class SceneWindow : public Window {

private:
    Scene *scene;

    DayScene *dayScene;
    MenuScene *menuScene;

    template<typename T>
    void initScene(T *currentScene);

public:
    SceneWindow();

    void onKey(int key, int scanCode, int action, int mods) override;

    void onIdle() override;

    void openDayScene();

    void openMenu();

};

#endif //FINALFINAL_SCENEWINDOW_H
