#include "main_window.h"
#include "scene.h"
#include "day_scene.h"
#include "menu_scene.h"

const unsigned int SIZE = 1024;

template<typename T>
void SceneWindow::initScene(T *currentScene) {
    currentScene->init();

    /* Handling Camera */
    auto camera = make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
    currentScene->camera = move(camera);

    this->scene = currentScene;
    this->scene->windowRef = this;
}

SceneWindow::SceneWindow() : Window{"Apartment", SIZE, SIZE} {
    this->dayScene = new DayScene();
    this->menuScene = new MenuScene();

    hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    this->openMenu();
}

void SceneWindow::onKey(int key, int scanCode, int action, int mods) {
    scene->keyboard[key] = action;
    scene->handleKey(key, action);
}


void SceneWindow::onIdle() {

    static auto time_ = (float) glfwGetTime();
    float dt = 0;
    this->scene->deltaTime = (float) glfwGetTime() - time_;

// Compute time delta
    if (dynamic_cast<DayScene *>(this->scene)) {
        dt = (float) glfwGetTime() - time_;
        dynamic_cast<DayScene *>(this->scene)->_weather->update();
    }


    glClearColor(.7f, .9f, .96f, 0);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->update(dt);
    scene->render();
}

void SceneWindow::openDayScene() {
    initScene(this->dayScene);
}

void SceneWindow::openMenu() {
    View menuView = {
            {0, 1, 0},
            {0, 0, -30},
            {0, 0, 0}
    };
    if (this->scene) {
        this->scene->camera->update(menuView);
    }
    initScene(this->menuScene);
}