#include "headers/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> pIndices, Texture pTexture, Shader& pshader) :
    shader(pshader), indices(pIndices), texture(pTexture) {
    vao.Bind();
    VBO vbo(vertices);
    EBO ebo(pIndices);

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Mesh::GiveInstancedData(unsigned int& matBuffer) {
    vao.Bind();
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);

    vao.Unbind();
}

void Mesh::Draw(Camera& cam) {
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cam.camMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    texture.texUnit(shader, "tex0", 0);
    texture.Bind();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::DrawInstanced(Camera& cam, int amount) {
    shader.Activate();
    glUniform1f(glGetUniformLocation(shader.ID, "time"), glfwGetTime());
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cam.camMatrix));
    texture.texUnit(shader, "tex0", 0);
    texture.Bind();
    vao.Bind();
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount);
}