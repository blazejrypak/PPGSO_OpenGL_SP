#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <bin/ppgso/lib/ppgso.h>

using namespace glm;
using namespace std;

class DayScene;

typedef struct {
    vec3 up;
    vec3 position;
    vec3 center;
} View;

class Camera {
private:

    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
    
    vec3 globalUp = vec3{0, 1, 0};

    uint view = 0;
    vector<View> views = {
//        {
//            globalUp,
//            {0.f, 5.f, -30.f},
//            {.0f, 0.f, .0f},
//        },
        {
            globalUp,
            {14.0f, 30, -60},
            {14.0f, 2, 14.0f},
        },
        {
            globalUp,
            {2,     4,  0},
            {0,     0, -1},
        }
    };

    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw;
    float pitch;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

public:
    
    View current = {
        globalUp,
        {0, 0, 0},
        {0, 0, 0},
    };

public:
    
    mat4 viewMatrix;
    mat4 projectionMatrix;
    
    vector<int> controls = {
        GLFW_KEY_UP,
        GLFW_KEY_DOWN,
        GLFW_KEY_RIGHT,
        GLFW_KEY_LEFT,
        GLFW_KEY_O,
        GLFW_KEY_P
    };
    
    
    Camera (float fow = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);
    
    void update ();
    
    void update (View view);
    
    void update (vec3 center);
    
    void update (vec3 eye, vec3 center);
    
    void updateWithDirection (vec3 position, short direction);

    vec3 getDirectionMatrix (short direction);
    
    vec3 cast (double u, double v);
    
    void switchView (DayScene *scene);
    
    void handleKey (int key);
    
    bool isFirstPersonMode ();
    
    View getView ();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

