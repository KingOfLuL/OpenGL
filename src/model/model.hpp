#ifndef MODEL_HPP
#define MODEL_HPP

#include <util/util.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <model/mesh.hpp>

using namespace std;

class Model
{
public:
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;

    Model(string const &path);
    Model();
    void draw(Shader &shader);

private:
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif // MODEL_HPP