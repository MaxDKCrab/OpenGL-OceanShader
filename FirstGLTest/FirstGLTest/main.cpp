#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "SHADER.h"
#include "CAMERA.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//define functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int textures();
int THREEDEE();

//cam variables for moving
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float fov = 45;

//deltatime things
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

//mouse variables
float lastX = 400, lastY = 300;
float yaw = 0;
float pitch = 0;
bool firstMouse = true;


#pragma region defaultBlock
////define vertex shader glsl source code
//const char* vertexShaderSource = "#version 330 core\n"
//"layout(location = 0) in vec3 aPos; \n"
//"layout(location = 1) in vec3 aColor; \n"
//"out vec3 ourColor; \n"
//"void main()\n"
//"{\n"
//"    gl_Position = vec4(aPos, 1.0); \n"
//"    ourColor = aColor; \n"
//"}\0";
//
////define fragment shader glsl source code
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor; \n"
//"in vec3 ourColor; \n"
//"void main()\n"
//"{\n"
//    "FragColor = vec4(ourColor, 1.0); \n"
//"}\0";

    //translation example

//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//glm::mat4 trans = glm::mat4(1.0f);
//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//vec = trans * vec;
//std::cout << vec.x << vec.y << vec.z << std::endl;



int main()
{

    THREEDEE();
    return 0;


    textures();
    return 0;

    //initialize glfw and set some settings
    glfwInit();
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //use core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //we create the glfw window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Bungus", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create BUNGUS" << std::endl;
        glfwTerminate();
        return -1;
    }
    //make sure the glfw window is our current context
    glfwMakeContextCurrent(window);

    //initialize glad before calling any gl funcs
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //set gl viewport size to same as glfw window, can be smaller
    glViewport(0, 0, 800, 600);


    // register all funcs before render loop
    // register resize window callback function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



    //create vao, bind vao, create verts,create vbo bind verts to vbo, set atrib pointers, unbind vao. Repeat for each new object
#pragma region createVaoObjs

//TRIANGLE

    //create a vertex array object to store our vertex buffers and vertex attribute pointers
    unsigned int TriangleVAO;
    glGenVertexArrays(1, &TriangleVAO);

    glBindVertexArray(TriangleVAO);

    //define vertex data
    float triVertices[] = {
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f
    };

    //Vertex buffer object defined
    unsigned int VBO;
    //vbo gets an index
    glGenBuffers(1, &VBO);
    //our vbo is now bound to the array buffer and it will be the one we use, we use the array buffer for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Copy the vertex data we defined into the array buffer which is bound to our vertex buffer object
    //static draw is for data defined once and that is used many times
    //dynamic draw is for data we need to change often and use a lot, dynamic draw gives it higher prio on gpu
    //stream draw likely takes up the least space but is slower to call on (i assume)
    glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);

    //specify how opengl interprets our vertex data
    //in the vertex shader source we specified 0 as the location of our vertex atribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbind our vao, saved for later use
    glBindVertexArray(0);


    //RECTANGLE

        //same process as before mostly

    unsigned int rectVAO;
    glGenVertexArrays(1, &rectVAO);

    glBindVertexArray(rectVAO);

    //only declaring unique verts
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  1.0f,0.0f,0.0f,
     0.5f, -0.5f, 0.0f,  1.0f,0.0f,0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f,0.0f,1.0f,
    -0.5f,  0.5f, 0.0f,  0.0f,0.0f,1.0f
    };

    //we use indices to skip defining more verts than we need
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };


    unsigned int rectVBO;
    glGenBuffers(1, &rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //elemental array buffer object will let us use our indices to draw stuff
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //same process as binding our vbo, but this thime its to the element array buffer instaed of the normal array buffer, and we use our indices instead
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

#pragma endregion


    //create shaders and program
//#pragma region vertex and fragment shaders
//    //create a shader object with an id
//    unsigned int vertexShader;
//    unsigned int fragmentShader;
//    //create object
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//    //attach our source code to our shader object
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    //compile our shader object
//    glCompileShader(vertexShader);
//    glCompileShader(fragmentShader);
//
//    //check if our source code compiled
//    int  vrtSuccess;
//    char vrtInfoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vrtSuccess);
//
//    if (!vrtSuccess)
//    {
//        glGetShaderInfoLog(vertexShader, 512, NULL, vrtInfoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vrtInfoLog << std::endl;
//    }
//
//    int  frgSuccess;
//    char frgInfoLog[512];
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &frgSuccess);
//
//    if (!frgSuccess)
//    {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, frgInfoLog);
//        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << frgInfoLog << std::endl;
//    }
//
//    //Create program with id ref
//    unsigned int shaderProgram;
//    shaderProgram = glCreateProgram();
//
//    //attach and link vert and frag shaders to program
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    //check if shader program compiled succesfully
//    int  prgSuccess;
//    char prgInfoLog[512];
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &prgSuccess);
//    if (!prgSuccess) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, prgInfoLog);
//        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << prgInfoLog << std::endl;
//    }
//
//#pragma endregion


    ////we allready attached our shaders to the program so we dont need these objects anymore
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    Shader ourShader("Shaders/vertShader.vert", "Shaders/fragShader.frag");


    float frames = 0;

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //do all my input things
        processInput(window);

        //render commands here
        //clear previous render commands in the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //retrieve our uniform value and use time to set a green value with sin
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        //this sets all rendering from now on to use our created shader program
        ourShader.use();

        //set the value of our uniform color variable
        //glUniform1f(vertexColorLocation,1.0f);
        ourShader.setFloat("ourColor", greenValue);

        if (frames > 120) {
            glBindVertexArray(rectVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //draw in wireframe
           // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            //do this again for some reason (i guess the reason is that we might wanna unbind it to do other shit(edit: yeah thats why))
            glBindVertexArray(TriangleVAO);
            //GIVE ME TRIANGLE PLEASE
            glDrawArrays(GL_TRIANGLES, 0, 3);
            //back to fill
           // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (frames > 240) frames = 0;


        frames++;


        //draw pixels on screen via double buffer (back buffer is the work zone and front buffer is the final product)
        glfwSwapBuffers(window);
        //poll events checks for input events and stuff like resizing the window and calls our callback funcs
        glfwPollEvents();
    }


    //properly exit if loop is broken
    glfwTerminate();
    return 0;
}
#pragma endregion


int textures()
{
    glfwInit();
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Bungus", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create BUNGUS" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


#pragma region createVaoObjs

    //TRIANGLE

    unsigned int TriangleVAO;
    glGenVertexArrays(1, &TriangleVAO);

    glBindVertexArray(TriangleVAO);

    float triVertices[] = {//positions,colors,texcoords
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.5f, 1.0f
    };

    unsigned int VBO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //unbind our vao, saved for later use
    glBindVertexArray(0);

#pragma endregion


#pragma region createTextures, blend them, apply them
    //create txture object, give it an id
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);
    //bind texture object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //define wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //define texture filtering mode, linear smooths, nearest chops
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //texture filter mode when downscaling can be done with mipmaps (which is good for performance and looks) thats why i use this instead
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //use stb image loader to load our jpg file and grab the width/height and channels from it
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        //generate a texture to our bound object with our previous data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //generate mipmaps for our function
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else { //shoot error if image didnt load correctly
        std::cout << "Failed to load texture 1" << std::endl;
    }

    //frees the image data from our loader
    stbi_image_free(data);
    //unbinds the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //do process for second texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //flip image on load if u want
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("Textures/Noise.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else { //shoot error if image didnt load correctly
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion

  
    //defining visible space

    //define a orthographic projection matrix with glm, first four are widht and height while the last two define the near and far plane
   //glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

    //define a perspective projection matrix with glm, first value is FOV, next is aspect ratio, last two are for the near and far plane 
   // glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);


    //definging the 3 matrixes we need to go from local to world to view to clip to screen space.
    //we only need to take our coordinates to clip space and gl does the last transformation and thats why we only need 3 since our first vector is predefined

    //the model matrix, rotating a bit
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    //the view matrix, moving back a bit
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    //for the projection matrix we are going with a perspective projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    Shader ourShader("Shaders/texVertShader.vert", "Shaders/texFragShader.frag");


    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.setFloat("time", glfwGetTime());
        
        ourShader.setM4("model", model); // to world space
        ourShader.setM4("view", view); //to view space
        ourShader.setM4("projection", projection); //to clip space

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(TriangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int THREEDEE() {
    glfwInit();
    //version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Bungus", NULL, NULL);

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

    glViewport(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader ourShader("Shaders/texVertShader.vert", "Shaders/texFragShader.frag");

#pragma region createVaoObjs

    //TRIANGLE

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);

    glBindVertexArray(cubeVAO);

    float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    unsigned int rectVBO;
    glGenBuffers(1, &rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

#pragma endregion


#pragma region createTextures, blend them, apply them
    //create txture object, give it an id
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);
    //bind texture object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //define wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //define texture filtering mode, linear smooths, nearest chops
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //texture filter mode when downscaling can be done with mipmaps (which is good for performance and looks) thats why i use this instead
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //use stb image loader to load our jpg file and grab the width/height and channels from it
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        //generate a texture to our bound object with our previous data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //generate mipmaps for our function
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else { //shoot error if image didnt load correctly
        std::cout << "Failed to load texture 1" << std::endl;
    }

    //frees the image data from our loader
    stbi_image_free(data);
    //unbinds the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //do process for second texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //flip image on load if u want
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("Textures/Noise.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else { //shoot error if image didnt load correctly
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion
 



    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    //things you would need to create a custom camera
   /* glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));*/


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  0.0f,  0.0f),
            glm::vec3(2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f,  2.0f, -2.5f),
            glm::vec3(1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        ourShader.setM4("view", view); //to view space
        ourShader.setM4("projection", projection); //to clip space

        ourShader.setFloat("time", glfwGetTime());

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            ourShader.setM4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}



//resize window callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//process input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;


    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
