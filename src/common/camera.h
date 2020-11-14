#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <bin/ppgso/lib/ppgso.h>

using namespace glm;
using namespace std;

class DayScene;
class Scene;

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

    std::vector<View> animationKeyFrames = {
            {
                globalUp,
                    {15.0f, 30.0f, -60.0f},
                    {15.0f, 2.0f, 15.0f},

            },
            {
                    globalUp,
                    {30.0f, 20.0f, -40.0f},
                    {15.0f, 2.0f, 15.0f},

            },
            {
                    globalUp,
                    {60.0f, 15.0f, -20.0f},
                    {15.0f, 2.0f, 15.0f},

            },
            {
                    globalUp,
                    {60.0f, 15.0f, 0.0f},
                    {15.0f, 2.0f, 15.0f},

            },
            {
                    globalUp,
                    {60.0f, 20.0f, 20.0f},
                    {15.0f, 2.0f, 15.0f},

            },
            {
                    globalUp,
                    {40.0f, 30.0f, 40.0f},
                    {15.0f, 2.0f, 15.0f},

            },
            {
                    globalUp,
                    {30.0f, 30.0f, 40.0f},
                    {15.0f, 2.0f, 15.0f},

            }
    };
    std::vector<View> animationFrames;
    int animationPathSteps = 10;

    static glm::vec3 lerp3(const glm::vec3 &p1, const glm::vec3 &p2, const float t) {
        return p1 * (1.f - t) + p2 * t;
    }

    glm::vec3 getAnimationPoint(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const float t){
        vec3 a = lerp3(p0, p1, t);
        vec3 b = lerp3(p1, p2, t);

        return lerp3(a, b, t);
    }

    void generateAnimationShape(){
        for (unsigned int i = 1; i < animationKeyFrames.size(); i+=2) {
            for (int j = 0; j <= animationPathSteps; ++j) {
                View frame;
                frame.position = getAnimationPoint(animationKeyFrames[i-1].position, animationKeyFrames[i].position, animationKeyFrames[i+1].position, static_cast<float>(j)/static_cast<float>(animationPathSteps));
                frame.center = getAnimationPoint(animationKeyFrames[i-1].center, animationKeyFrames[i].center, animationKeyFrames[i+1].center, static_cast<float>(j)/static_cast<float>(animationPathSteps));
                frame.up = getAnimationPoint(animationKeyFrames[i-1].up, animationKeyFrames[i].up, animationKeyFrames[i+1].up, static_cast<float>(j)/static_cast<float>(animationPathSteps));
                animationFrames.emplace_back(frame);
            }
        }
    }

    void resetAnimation();

public:
    bool animationRunning;
    float animationDeltaTime;
    float animationFramesPerSecond;
    unsigned int currentAnimationFrameIndex;
    vec3 firstPersonViewCenter{0, 0, 0};
    
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
        GLFW_KEY_P,
        GLFW_KEY_X
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
    
    void handleKey (Scene &scene, int key);
    
    bool isFirstPersonMode ();
    
    View getView ();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

