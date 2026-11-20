#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 camMatrix = glm::mat4(1.0f);
        float windowWidth;
        float windowHeight;
        float FOVdeg;
        float nearPlane;
        float farPlane;
        float sensitivity = 100.0f;
        float speed = 0.07f;
        float playerHeight = 2.0f;
        bool firstClick = true;

        Camera(float pwindowWidth, float pwindowHeight, float pFOVdeg, float pnearPlane, float pfarPlane);
        void updateMatrix();
        void freeCam(GLFWwindow* window);
        void firstPersonCam(GLFWwindow* window);
};

#endif