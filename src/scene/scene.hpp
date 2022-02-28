#ifndef SCENE_HPP
#define SCENE_HPP

#include "pch/pch.hpp"
#include "model/model.hpp"
#include "shader/shader.hpp"
#include "camera/camera.hpp"

class Scene
{
public:
    Scene();

    Model backpack;
    Model cube;
    Shader shaderLit;
    Shader shaderColor;

    void draw(const Camera &camera, float SCR_WIDTH, float SCR_HEIGHT, float deltaTime);

private:
    glm::vec3 initailPointLightPositions[4] = {
        glm::vec3(1.8f, -0.2f, 0.4f),
        glm::vec3(-1.0f, 0.5f, 1.3f),
        glm::vec3(-1.7f, 0.6f, -1.6f),
        glm::vec3(1.2f, -0.9f, -1.4f)};
    glm::vec3 pointLightPositions[4];

    glm::vec3 pointLightColors[4] = {
        glm::vec3(0.9, 0.2, 0.5),
        glm::vec3(0.4, 0.3, 1.0),
        glm::vec3(0.2, 0.7, 0.8),
        glm::vec3(0.6, 1.0, 0.3)};

    float pointLightAngles[4] = {0.0f, 90.0f, 180.0f, 270.0f};
    glm::vec3 dirLightDirection = glm::vec3(0.5f, 0.4f, -0.2f);
};

#endif // SCENE_HPP