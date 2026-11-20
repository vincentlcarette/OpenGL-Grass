#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"

class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Texture texture, Shader& shader);
        void Draw(Camera& cam);
        void GiveInstancedData(unsigned int& matBuffer);
        void DrawInstanced(Camera& cam, int amount);
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        Shader& shader;
        Texture texture;

    private:
        VAO vao;
        std::vector<GLuint> indices;
};

#endif