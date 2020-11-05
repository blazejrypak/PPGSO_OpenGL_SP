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

using namespace std;
using namespace glm;
using namespace ppgso;

class SceneWindow : public Window {

private:
    Menu *menu;
    Scene *scene;

    DayScene *dayScene;

    template<typename T>
    void initScene (T *currentScene);

public:
    SceneWindow ();
    
    void onKey (int key, int scanCode, int action, int mods) override;
    
    void onIdle () override;
    
    void openDayScene ();
    
    void startGame ();
    
};

#endif //FINALFINAL_SCENEWINDOW_H
