#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <shader/shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

using namespace std;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};
struct Texture
{
    unsigned int id;
    string path;
    string type;
};

class Mesh
{
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void draw(Shader &shader);

private:
    unsigned int VBO, EBO;

    void setupMesh();
};

#endif // MESH_HPP