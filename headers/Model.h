#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include"Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    public:
        Model(char* path, Shader& pshader) : shader(pshader) {
            loadModel(path);
        }
        void Draw(Camera& cam);
        void DrawInstanced(Camera& cam, int amount);
        void GiveInstancedData(int amount, std::vector<glm::mat4>& matrices);
    private:
        std::string directory;
        Shader& shader;
        Assimp::Importer import;
        std::vector<Mesh> meshes;
        Texture testText = Texture("brick.png", 0);

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif