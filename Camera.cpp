#include"headers/Camera.h"

Camera::Camera(float pwindowWidth, float pwindowHeight, float pFOVdeg, float pnearPlane, float pfarPlane) :
    windowWidth(pwindowWidth), windowHeight(pwindowHeight), FOVdeg(pFOVdeg), nearPlane(pnearPlane), farPlane(pfarPlane) {

}

void Camera::updateMatrix() {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);
    
    view = glm::lookAt(position, position + orientation, up);
    proj = glm::perspective(glm::radians(FOVdeg), windowWidth / windowHeight, nearPlane, farPlane);

    camMatrix = proj * view;
}

void Camera::firstPersonCam(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	double mouseX;
	double mouseY;

	glfwGetCursorPos(window, &mouseX, &mouseY);

	float rotX = sensitivity * (float)(mouseY - (windowHeight / 2)) / windowHeight;
	float rotY = sensitivity * (float)(mouseX - (windowWidth / 2)) / windowWidth;

	glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

	if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
		orientation = newOrientation;
	}

	orientation = glm::rotate(orientation, glm::radians(-rotY), up);
	glfwSetCursorPos(window, (windowWidth / 2), (windowHeight / 2));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += glm::normalize(orientation * glm::vec3(1.0f, 0.0f, 1.0f)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= glm::normalize(orientation * glm::vec3(1.0f, 0.0f, 1.0f)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += glm::normalize(glm::cross(up, orientation)) * glm::vec3(1.0f, 0.0f, 1.0f) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position -= glm::normalize(glm::cross(up, orientation)) * glm::vec3(1.0f, 0.0f, 1.0f) * speed;
    }

	position.y = playerHeight;
}

void Camera::freeCam(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += orientation * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= orientation * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += glm::normalize(glm::cross(up, orientation)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position -= glm::normalize(glm::cross(up, orientation)) * speed;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			glfwSetCursorPos(window, (windowWidth / 2), (windowHeight / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (windowHeight / 2)) / windowHeight;
		float rotY = sensitivity * (float)(mouseX - (windowWidth / 2)) / windowWidth;

		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			orientation = newOrientation;
		}

		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		glfwSetCursorPos(window, (windowWidth / 2), (windowHeight / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}