#include <glm/glm.hpp>

#include "camera.h"
#include "day_scene.h"

using namespace std;
using namespace glm;
using namespace ppgso;


Camera::Camera (float fow, float ratio, float near, float far) {
    float fowInRad = (PI / 180.0f) * fow;
    projectionMatrix = perspective(fowInRad, ratio, near, far);
    worldUp = this->views[this->view].up;
    this->update(this->views[this->view]);
}


void Camera::update () {
    this->update(current.position, current.center);
}

void Camera::update (vec3 eye, vec3 center) {
    viewMatrix = lookAt(eye, center + front, globalUp);
}

void Camera::update (View view) {
    current = view;
    this->update();
}

void Camera::update (vec3 center) {
    viewMatrix = lookAt(current.position, center + front, globalUp);
}

void Camera::updateWithDirection (vec3 position, short direction) {
    current.position = position - this->getDirectionMatrix(direction);
    current.center = position;
    this->update();
}

vec3 Camera::getDirectionMatrix (short direction) {
    switch (direction) {
        case 0: {
            return vec3{0, -15, 6};
        }
        case 3: {
            return vec3{0, -15, -6};
        }
        case 1: {
            return vec3{6, -15, 0};
        }
        case 2: {
            return vec3{-6, -15, 0};
        }
    }
}

void Camera::switchView (DayScene *scene) {
    view++;
    if (view == views.size()) {
        view = 0;
    };
    if (view == 1) {
        views[view].position = scene->player->position - this->getDirectionMatrix(scene->player->direction);

        views[view].center = scene->player->position;
    }
    this->update(views[view]);
}

void Camera::handleKey (int key) {
    
    // Rotating camera shouldn't be enabled in third person mode
    if (view != 1) {
        float speed = 0.2f;
        float speedRotation = 2.5f;

        if (key == GLFW_KEY_O) {
            current.position -= (current.center - current.position) * (speed / 2);
        }
        if (key == GLFW_KEY_P) {
            current.position += (current.center - current.position) * (speed / 2);
        }

        if (key == GLFW_KEY_UP) {
            current.center.y += speedRotation;
        }

        if (key == GLFW_KEY_DOWN) {
            current.center.y -= speedRotation;
        }

        if (key == GLFW_KEY_LEFT) {
            current.center.x += speedRotation;
        }

        if (key == GLFW_KEY_RIGHT) {
            current.center.x -= speedRotation;
        }
        this->update();
    }
};


vec3 Camera::cast (double u, double v) {
    // Create point in Screen coordinates
    vec4 screenPosition{u, v, 0.0f, 1.0f};
    
    // Use inverse matrices to get the point in world coordinates
    auto invProjection = inverse(projectionMatrix);
    auto invView = inverse(viewMatrix);
    
    // Compute position on the camera plane
    auto planePosition = invView * invProjection * screenPosition;
    planePosition /= planePosition.w;
    
    // Create direction vector
    auto direction = normalize(planePosition - vec4{current.position, 1.0f});
    return vec3{direction};
}

bool Camera::isFirstPersonMode () {
    return view == 1;
}

View Camera::getView () {
    return this->current;
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        current.position += front * velocity;
    if (direction == BACKWARD)
        current.position -= front * velocity;
    if (direction == LEFT)
        current.position -= right * velocity;
    if (direction == RIGHT)
        current.position += right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new front vector
    glm::vec3 front_;
    front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_.y = sin(glm::radians(pitch));
    front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front_);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, current.up));
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    current.up    = glm::normalize(glm::cross(right, front));
}