#include "scene.hpp"

Scene::Scene()
    : backpack("backpack/backpack.obj"), cube("cube/cube.obj"),
      shaderLit("vertex/vertexShader.glsl", "fragment/lit.glsl"), shaderColor("vertex/vertexShader.glsl", "fragment/color.glsl")
{
}

void Scene::draw(const Camera &camera, float SCR_WIDTH, float SCR_HEIGHT, float deltaTime)
{
    /// matrices
    glm::mat4 modelMatrix(1.0f);
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // update light positions
    for (int i = 0; i < 4; i++)
    {
        float x, y, z;
        if (i % 2 == 0)
        {
            x = sin(pointLightAngles[i]) * 2;
            y = cos(pointLightAngles[i]) * 2;
            z = cos(pointLightAngles[i]) * 2;
        }
        else
        {
            x = -cos(pointLightAngles[i]) * 2;
            y = -sin(pointLightAngles[i]) * 2;
            z = sin(pointLightAngles[i]) * 2;
        }
        pointLightPositions[i] = initailPointLightPositions[i] + glm::vec3(x, y, z);
        pointLightAngles[i] += deltaTime * 2;
    }

    shaderColor.use();

    shaderColor.setMat4("view", viewMatrix);
    shaderColor.setMat4("projection", projectionMatrix);

    for (int i = 0; i < 4; i++)
    {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, pointLightPositions[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
        shaderColor.setMat4("model", modelMatrix);
        shaderColor.setVec3("color", pointLightColors[i]);
        cube.draw(shaderColor);
    }
    modelMatrix = glm::mat4(1.0f);

    shaderLit.use();

    shaderLit.setMat4("view", viewMatrix);
    shaderLit.setMat4("projection", projectionMatrix);
    shaderLit.setMat4("model", modelMatrix);

    shaderLit.setVec3("viewPos", camera.position);
    shaderLit.setFloat("material.shininess", 32.0f);

    shaderLit.setVec3("dirLight.direction", dirLightDirection);
    shaderLit.setVec3("dirLight.ambient", glm::vec3(0.0f));
    shaderLit.setVec3("dirLight.diffuse", glm::vec3(0.2f));
    shaderLit.setVec3("dirLight.specular", glm::vec3(0.0f));
    for (int i = 0; i < 4; i++)
    {
        const std::string s = "pointLights[" + std::to_string(i) + "].";
        shaderLit.setVec3(s + "position", pointLightPositions[i]);
        shaderLit.setVec3(s + "ambient", glm::vec3(0.0f));
        shaderLit.setVec3(s + "diffuse", pointLightColors[i]);
        shaderLit.setVec3(s + "specular", pointLightColors[i]);
        shaderLit.setFloat(s + "constant", 1.0f);
        shaderLit.setFloat(s + "linear", 0.09f);
        shaderLit.setFloat(s + "quadratic", 0.032f);
    }
    backpack.draw(shaderLit);
}