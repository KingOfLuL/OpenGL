#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <camera/camera.hpp>
#include <shader/shader.hpp>
#include <model/model.hpp>
#include <util/util.hpp>

#include <vertices.hpp>

#include <iostream>

unsigned int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

glm::vec3 initailPointLightPositions[] = {
    glm::vec3(1.8f, -0.2f, 0.4f),
    glm::vec3(-1.0f, 0.5f, 1.3f),
    glm::vec3(-1.7f, 0.6f, -1.6f),
    glm::vec3(1.2f, -0.9f, -1.4f)};
glm::vec3 pointLightPositions[4];

glm::vec3 pointLightColors[] = {
    glm::vec3(0.9, 0.2, 0.5),
    glm::vec3(0.4, 0.3, 1.0),
    glm::vec3(0.2, 0.7, 0.8),
    glm::vec3(0.6, 1.0, 0.3)};

float pointLightAngles[] = {0.0f, 90.0f, 180.0f, 270.0f};

glm::vec3 dirLightDirection = glm::vec3(0.5f, 0.4f, -0.2f);
float dirLightAngle = 0.0f;

float deltaTime = 0;
float lastFrame = 0;

float lastX;
float lastY;
int firstMouse = 1;

Camera camera;

int main()
{
    stbi_set_flip_vertically_on_load(true);

    std::vector<glm::vec3> windows{
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)};

    lastX = SCR_WIDTH * 0.5;
    lastY = SCR_HEIGHT * 0.5;

    camera = Camera(glm::vec3(0.0f, 2.0f, 5.0f));

    // window setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to inti GLAD" << std::endl;
        return 1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // shader
    Shader shaderLit("vertex/vertexShader.glsl", "fragment/lit.glsl");
    Shader shaderUnlit("vertex/vertexShader.glsl", "fragment/unlit.glsl");
    Shader shaderColor("vertex/vertexShader.glsl", "fragment/color.glsl");

    // model
    Model backpack("backpack/backpack.obj");
    Model cube("cube/cube.obj");
    Model plane("plane/plane.obj");

    unsigned int transparentTex = util::textureFromFile("window.png", util::TEXTURE_PATH);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // background
        glClearColor(0.1, 0.1, 0.1, 1.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        shaderUnlit.use();
        shaderUnlit.setMat4("view", viewMatrix);
        shaderUnlit.setMat4("projection", projectionMatrix);

        std::map<float, glm::vec3> sorted;

        glActiveTexture(GL_TEXTURE0);
        shaderUnlit.setInt("material.diffuse", 0);
        glBindTexture(GL_TEXTURE_2D, transparentTex);
        for (unsigned int i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(camera.position - windows[i]);
            sorted[distance] = windows[i];
        }
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, it->second);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
            shaderUnlit.setMat4("model", modelMatrix);
            plane.draw(shaderUnlit);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.processKeyboard(DOWN, deltaTime);
}
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn)
{
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    camera.processMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    camera.processMouseScroll(static_cast<float>(yOffset));
}