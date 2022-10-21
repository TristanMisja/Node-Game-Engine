#ifndef _H_CAMERA_
#define _H_CAMERA_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    float pitch;
    float yaw;
    float speed;
    float sensitivity;
    float FOV;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Constructor with vectors
    Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = 45.0f, float _pitch = 0.0f) : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(2.5f), sensitivity(0.07f), FOV(90.0f)
    {
        position = position;
        worldUp = _up;
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f, float _yaw = 45.0f, float _pitch = 0.0f) : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(2.5f), sensitivity(0.07f), FOV(90.0f)
    {
        position = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        yaw = _yaw;
        pitch = _pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = speed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
    }

    void ProcessKeyboard(Camera_Movement direction)
    {
        if (direction == FORWARD)
            position += front * speed;
        if (direction == BACKWARD)
            position -= front * speed;
        if (direction == LEFT)
            position -= right * speed;
        if (direction == RIGHT)
            position += right * speed;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    /*void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }*/

    float getPitch() { return pitch; }
    void setPitch(float _pitch) { pitch = _pitch; }
    float getYaw() { return yaw; }
    void setYaw(float _yaw) { yaw = _yaw; }
    float getSpeed() { return speed; }
    void setSpeed(float _speed) { speed = _speed; }
    float getSensitivity() { return sensitivity; }
    void setSensitivity(float _sensitivity) { sensitivity = _sensitivity; }
    float getFOV() { return FOV; }
    void setFOV(float _FOV) { FOV = _FOV; }

    glm::vec3 getPosition() { return position; }
    void setPosition(glm::vec3 _position) { position = _position; }
    void setPosition(float* _position) { position = glm::vec3(_position[0], _position[1], _position[2]); }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the front vector
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.w
        up = glm::normalize(glm::cross(right, front));
    }
};
#endif