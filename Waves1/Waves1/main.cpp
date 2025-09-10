#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "SHADER.h"
#include "CAMERA.h"
#include <random>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 800;
const unsigned int planeWidth = 500;
const unsigned int waveCount = 500;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//deltatime things
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//lighting
glm::vec3 lightPos(0, 4, -10);


//plane generation
float vertices[planeWidth * planeWidth * 6 * 3];

float quadLength = 2 / (float)planeWidth;

float lowEnd = -1;

float xOffset = lowEnd;
float zOffset = lowEnd;

int vertIndex = 0;

//WaveGeneration

std::vector<glm::vec4> generateRandomWaves(size_t count) {
    std::vector<glm::vec4> waves;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> steepnessDist(0.1, 0.3);
    std::uniform_real_distribution<float> waveLengthDist(20, 80);
    std::uniform_real_distribution<float> directionDist(-100, 100);


    for (size_t i = 0; i < count; i++)
    {   

        float steep = steepnessDist(gen);
        float waveL = waveLengthDist(gen);
        float directionX = directionDist(gen);
        float directionY = directionDist(gen);
        waves.emplace_back(directionX, directionY, steep, waveL);

    }

    return waves;
}





int main() {

//SETUP
    glfwInit();
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bungus", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create BUNGUS" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader waveShader("Shaders/WaveShader.vert", "Shaders/WaveShader.frag");
    Shader lightShader("Shaders/LightSourceShader.vert", "Shaders/LightSourceShader.frag");


 //SETUP PLANE

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);



    for (size_t i = 0; i < planeWidth; i++)
    {
        float xHighVal = xOffset + quadLength;
        float xLowVal = xOffset;
        
        for (size_t i = 0; i < planeWidth; i++)
        {
            float zHighVal = zOffset + quadLength;
            float zLowVal = zOffset;

            vertices[vertIndex] = xHighVal;  vertIndex++; vertices[vertIndex] = 0.0;  vertIndex++; vertices[vertIndex] = zHighVal;  vertIndex++;
            vertices[vertIndex] = xHighVal;  vertIndex++; vertices[vertIndex] = 0.0;  vertIndex++; vertices[vertIndex] = zLowVal;  vertIndex++;
            vertices[vertIndex] = xLowVal;  vertIndex++; vertices[vertIndex] = 0.0;  vertIndex++; vertices[vertIndex] = zLowVal;  vertIndex++;
            vertices[vertIndex] = xLowVal;  vertIndex++; vertices[vertIndex] = 0.0;  vertIndex++; vertices[vertIndex] = zLowVal;  vertIndex++;
            vertices[vertIndex] = xLowVal;  vertIndex++; vertices[vertIndex] = 0.0;  vertIndex++; vertices[vertIndex] = zHighVal;  vertIndex++;
            vertices[vertIndex] = xHighVal;  vertIndex++; vertices[vertIndex] = 0.0;  vertIndex++; vertices[vertIndex] = zHighVal;  vertIndex++;


            zOffset += quadLength;
        }

        zOffset = lowEnd;
        xOffset += quadLength;
    }

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

//setup light source

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);


    float cubeVertices[] = {
-0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
-0.5f,  0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,

-0.5f, -0.5f,  0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,
-0.5f, -0.5f,  0.5f,

-0.5f,  0.5f,  0.5f,
-0.5f,  0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,
-0.5f, -0.5f, -0.5f,
-0.5f, -0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,

 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,

-0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, -0.5f,  0.5f,
 0.5f, -0.5f,  0.5f,
-0.5f, -0.5f,  0.5f,
-0.5f, -0.5f, -0.5f,

-0.5f,  0.5f, -0.5f,
 0.5f,  0.5f, -0.5f,
 0.5f,  0.5f,  0.5f,
 0.5f,  0.5f,  0.5f,
-0.5f,  0.5f,  0.5f,
-0.5f,  0.5f, -0.5f
    };



    unsigned int lightVBO;
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


//before render loop
    
    waveShader.use();

    std::vector<glm::vec4> waves = generateRandomWaves((size_t)waveCount);

    for (size_t i = 0; i < (size_t)waveCount; i++)
    {
        std::string name = "waves[" + std::to_string(i) + "]";
        //glUniform4f(glGetUniformLocation(waveShader.ID, name.c_str()),waves[i].x,waves[i].y,waves[i].z,waves[i].w);

        waveShader.setVec4(name.c_str(), waves[i]);
    }

//RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //deltaTime calcs
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        waveShader.use();

        waveShader.setVec3("objectColor", 0.05f, 0.36f, 0.61f);
        waveShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        waveShader.setVec3("lightPos", lightPos);
        waveShader.setVec3("viewPos", camera.Position);
        

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0,-1.0,0.0));
        model = glm::scale(model, glm::vec3(50));
       // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
       
        //TO FIGURE OUT
        //I generate a normal matrix by casting my model matrix into a 3x3 and taking the transpose inverse of it
        //This breaks my diffuse lighting; more comments in my vertex shader
        //glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        //waveShader.setM3("normalMatrix", normalMatrix);
        waveShader.setM4("model", model); //to world space
        waveShader.setM4("view", view); //to view space
        waveShader.setM4("projection", projection); //to clip space
        waveShader.setFloat("time", glfwGetTime());

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, planeWidth * planeWidth * 6);

       //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        lightShader.use();


        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));


        lightShader.setM4("model", lightModel); //to world space
        lightShader.setM4("view", view); //to view space
        lightShader.setM4("projection", projection); //to clip space

        glBindVertexArray(lightVAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


//FUNCTIONS for camera and other shit
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard((Camera_Movement)0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard((Camera_Movement)1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard((Camera_Movement)2, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard((Camera_Movement)3, deltaTime);

    
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
