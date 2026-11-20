#include"headers/Model.h"

void Model::loadModel(std::string path) {
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Assimp error: " << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Texture texture;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        } else {
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        vertex.color = glm::vec3(1.0f, 1.0f, 1.0f);

        if (mesh->mTextureCoords[0]) {
            vertex.texUV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int k = 0; k < face.mNumIndices; k++) {
            indices.push_back(face.mIndices[k]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> textures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        if (textures.size() > 0) {
            texture = textures[0];
        } else {
            texture = testText;
        }
    }

    return Mesh(vertices, indices, texture, shader);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture((directory + '/' + str.C_Str()).c_str(), 0);
        textures.push_back(texture);
    }
    return textures;
}

void Model::GiveInstancedData(int amount, std::vector<glm::mat4>& matrices) {
    unsigned int matBuffer;
    glGenBuffers(1, &matBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, matBuffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].GiveInstancedData(matBuffer);
    }
}

void Model::Draw(Camera& cam) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(cam);
    }
}
void Model::DrawInstanced(Camera& cam, int amount) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].DrawInstanced(cam, amount);
    }
}