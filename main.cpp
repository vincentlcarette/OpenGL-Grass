#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<cmath>

#include "headers/Model.h"

int rows = 150;
int cols = 150;
int amountOfGrass = rows * cols;
std::vector<glm::mat4> matrices;

float windowWidth = 800;
float windowHeight = 800;

int getRandomInt(int min, int max) {
	return min + (std::rand() % (max - min + 1));
}

float clamp(float num, float min, float max) {
    if (num > max) {return max;}
    if (num < min) {return min;}
    return num;
}

std::vector<std::vector<float>> generateWorleyNoise(int width, int height, int amountOfPoints) {
    std::vector<std::vector<float>> generatedNoise;
    std::vector<std::vector<int>> points;

    for (int i = 0; i < amountOfPoints; i++) {
        std::vector<int> point = {getRandomInt(0, width), getRandomInt(0, height)};
        points.push_back(point);
    }

    for (int row = 0; row < height; row++) {
        std::vector<float> newCol;
        for (int col = 0; col < width; col++) {
            float lowestDistance = sqrt((col - points[0][0]) * (col - points[0][0]) + (row - points[0][1]) * (row - points[0][1]));
            for (std::vector<int> point : points) {
                lowestDistance = std::min(lowestDistance, (float) sqrt((col - point[0]) * (col - point[0]) + (row - point[1]) * (row - point[1])));
            }
            newCol.push_back(clamp(lowestDistance / 100.0f, 0, 1));
        }
        generatedNoise.push_back(newCol);
    }

    return generatedNoise;
}

int main() {
	std::srand(time(0));
	std::vector<std::vector<float>> noise = generateWorleyNoise(cols, rows, 10);

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			glm::mat4 mat(1.0f);
			float height = clamp(noise[row][col] * 3, 0.6f, 50.0f);
			mat = glm::scale(mat, glm::vec3(1.0f, height, 1.0f));
			mat = glm::translate(mat, glm::vec3(
				(float) col * (100.0f / (float) cols) - 49.0f + (100.0f / (float) cols / 2.0f) + (float) getRandomInt(-100, 100) / 100.0f,
				1.0f,
				(float) row * (100.0f / (float) rows) - 49.0f + (100.0f / (float) rows / 2.0f) + (float) getRandomInt(-100, 100) / 100.0f
			));
			matrices.push_back(mat);
		}
	}
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Grass", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	gladLoadGL();

	glViewport(0, 0, windowWidth, windowHeight);

	Shader shaderProgram("default.vert", "default.frag");
	Shader grassShader("grass.vert", "grass.frag");

	Camera cam(windowWidth, windowHeight, 70.0f, 0.1f, 100.0f);
	cam.position = glm::vec3(0.0f, 0.3f, 2.0f);
	Model floor("floorModel/scene.gltf", shaderProgram);
	Model thing("grassModel/scene.gltf", grassShader);

	thing.GiveInstancedData(amountOfGrass, matrices);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cam.firstPersonCam(window);
		cam.updateMatrix();
		floor.Draw(cam);
		thing.DrawInstanced(cam, amountOfGrass);

		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			break;
		}
	}

	shaderProgram.Delete();
	grassShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}