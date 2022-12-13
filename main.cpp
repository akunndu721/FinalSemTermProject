// Std. Includes
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <filesystem>

// GL includes
#include "graphics_headers.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "circle.h"
#include "skybox.h"

using Circle = Learus_Circle::Circle;
using Skybox = Learus_Skybox::Skybox;

// Properties
const GLuint WIDTH = 1600, HEIGHT = 1200;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Some settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// Timing
float lastFrame = 0.0f;
float frameToggled = 0.0f;
float timeSinceLastToggle = 1.0f;

float earthOrbitRadius =100.0f;
float moonOrbitRadius = -20.0f;
glm::vec3 sunPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);

// Globals
bool animation = false;

// Camera
float cameraOrbitRadius = 30.0f;
float rotateAngle = 1.0f;

// Mouse Input
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;



//void framebufferSizeCallback(GLFWwindow* window, int width, int height);
//void mouseInput(GLFWwindow* window, double xpos, double ypos);
//void scrollInput(GLFWwindow* window, double xoffset, double yoffset);
//void keyboardInput(GLFWwindow* window, float deltaTime);


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void SphereVertices();
void Sphere();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastXX = 400, lastYY = 300;
bool frstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrm = 0.0f;

int numVertices;
int numIndices;
std::vector<int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> texCoords;
std::vector<glm::vec3> normals;
std::vector<glm::vec3> colors;


int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System - Term Project", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
  //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
 
   
    // Setup and compile our shaders
     //   Shader shader("res/shaders/cube.vs", "res/shaders/cube.frag");
    Shader planetShader("res/shaders/planet.vs", "res/shaders/planet.frag");
    Shader sunShader("res/shaders/sun.vs", "res/shaders/sun.frag");
    Shader skyboxShader("res/shaders/skybox.vs", "res/shaders/skybox.frag");

    // Load the models
    Model Sun("res/Planet/planet.obj");
    Model Earth("res/Earth/Globe.obj");
    Model Mercury("res/Planet/SpaceShip-1.obj");
    Model Moon("res/Rock/rock.obj");

    Circle EarthOrbitCircle(sunPos, earthOrbitRadius, glm::vec3(1.0f, 1.0f, 1.0f), 3000);
    Circle MoonOrbitCircle(earthPos, moonOrbitRadius, glm::vec3(1.0f, 1.0f, 1.0f), 3000);

    GLfloat cubeVertices[] =
    {
        // Positions          // Texture Coords
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

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    Sphere();
//    SphereVertices();

    // Setup cube VAO
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);

    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    // Load textures
    GLuint cubeTexture = TextureLoading::LoadTexture("res/images/container2.png");

    // Cubemap (Skybox)
    vector<const GLchar*> faces;
    faces.push_back("res/images/skybox1/right.png");
    faces.push_back("res/images/skybox1/left.png");
    faces.push_back("res/images/skybox1/top.png");
    faces.push_back("res/images/skybox1/bottom.png");
    faces.push_back("res/images/skybox1/front.png");
    faces.push_back("res/images/skybox1/back.png");
    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

/*
    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        timeSinceLastToggle += deltaTime;
        if (animation)
            frameToggled += deltaTime;


        keyboardInput(window, deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // view / projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        skyBox.setUniforms(projection, glm::mat4(glm::mat3(view)));
        skyBox.Draw();

        // Render the sun object
        sunShader.Use();
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);

        model = glm::translate(model, sunPos); // Center it (kinda)
        sunShader.setMat4("model", model);
        Sun.Draw(sunShader);


        planetShader.Use();

        // Set the lighting
        planetShader.setVec3("viewPos", camera.Position);
        planetShader.setFloat("material.shininess", 32.0f);

        planetShader.setVec3("light.position", sunPos);
        planetShader.setVec3("light.ambient", 0.25f, 0.25f, 0.25f);
        planetShader.setVec3("light.diffuse", 1.8f, 1.8f, 1.8f);
        planetShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[0].constant", 1.0f);
        planetShader.setFloat("pointLights[0].linear", 0.045);
        planetShader.setFloat("pointLights[0].quadratic", 0.0075);


        // Render the Earth object
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));


        // Orbit around the sun
        earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);
        model = glm::translate(model, earthPos);
        // Rotate around itself
        model = glm::rotate(model, frameToggled * 1.5f * glm::radians(-50.0f), glm::vec3(0.1f, 1.0f, 0.0f));

        planetShader.setMat4("model", model);
        Earth.Draw(planetShader);

        // Draw a circle showing the earth's orbit around the sun
        EarthOrbitCircle.setUniforms(projection, view);
        EarthOrbitCircle.scale(glm::vec3(0.1f, 0.1f, 0.1f));
        EarthOrbitCircle.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        EarthOrbitCircle.Draw();


        // Render the Moon object
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        // Orbit around the earth
        glm::vec3 moonPos = earthPos + glm::vec3(0.0f, sin(frameToggled) * moonOrbitRadius, cos(frameToggled) * moonOrbitRadius);
        model = glm::translate(model, moonPos);
        planetShader.Use();
        planetShader.setMat4("model", model);
        Moon.Draw(planetShader);

        // Draw a circle showing the moon's orbit around the earth
        MoonOrbitCircle.setUniforms(projection, view);
        MoonOrbitCircle.scale(glm::vec3(0.1f, 0.1f, 0.1f));
        MoonOrbitCircle.translate(earthPos);
        MoonOrbitCircle.rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        MoonOrbitCircle.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    */

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);


        //// Draw our first triangle
        //shader.Use();

        //// Bind Textures using texture units
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, cubeTexture);
        //glUniform1i(glGetUniformLocation(shader.Program, "texture1"), 0);

        //// Get the uniform locations
        //GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        //GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        //GLint projLoc = glGetUniformLocation(shader.Program, "projection");

        //// Pass the matrices to the shader
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //glBindVertexArray(cubeVAO);

        //// Calculate the model matrix for each object and pass it to shader before drawing
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //glBindVertexArray(0);


        // Draw skybox as last
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS); // Set depth function back to default

        // Render the sun object
        sunShader.Use();
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);

        model = glm::translate(model, sunPos); // Center it (kinda)l
        model *= glm::scale(glm::vec3(0.10, 0.10, 0.10));
        sunShader.setMat4("model", model);
        Sun.Draw(sunShader);



        planetShader.Use();

        // Set the lighting
        planetShader.setVec3("viewPos", camera.position);
        planetShader.setFloat("material.shininess", 32.0f);

        planetShader.setVec3("light.position", sunPos);
        planetShader.setVec3("light.ambient", 0.25f, 0.25f, 0.25f);
        planetShader.setVec3("light.diffuse", 1.8f, 1.8f, 1.8f);
        planetShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        planetShader.setFloat("pointLights[0].constant", 1.0f);
        planetShader.setFloat("pointLights[0].linear", 0.045);
        planetShader.setFloat("pointLights[0].quadratic", 0.0075);

        // Render the Earth object
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));


        // Orbit around the sun

        model = glm::mat4(1.0f);
        earthPos = glm::vec3(-1.0f, 0.0f, 1.0f);
 //       earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);
        model = glm::translate(model, earthPos);
        model *= glm::scale(glm::vec3(0.01, 0.01, 0.01));
        // Rotate around itself
        model = glm::rotate(model, frameToggled * 1.5f * glm::radians(-50.0f), glm::vec3(0.1f, -1.0f, 0.0f));

        planetShader.setMat4("model", model);
        Earth.Draw(planetShader);

        // Draw a circle showing the earth's orbit around the sun
        EarthOrbitCircle.setUniforms(projection, view);
        EarthOrbitCircle.scale(glm::vec3(0.05f, 0.05f, 0.05f));
        EarthOrbitCircle.translate(earthPos);
        EarthOrbitCircle.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        EarthOrbitCircle.Draw();

        // Orbit around the sun

        planetShader.Use();

        model = glm::mat4(1.0f);
        earthPos = glm::vec3(-1.1f, 0.0f, -1.1f);
        //       earthPos = sunPos + glm::vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);
        model = glm::translate(model, earthPos);
        model *= glm::scale(glm::vec3(0.05, 0.05, 0.05));
        // Rotate around itself
        model = glm::rotate(model, frameToggled * 1.5f * glm::radians(-50.0f), glm::vec3(0.1f, -1.0f, 0.0f));

        planetShader.setMat4("model", model);
        Moon.Draw(planetShader);

        // Draw a circle showing the moon's orbit around the earth
        MoonOrbitCircle.setUniforms(projection, view);
        MoonOrbitCircle.scale(glm::vec3(0.1f, 0.1f, 0.1f));
        MoonOrbitCircle.translate(earthPos);
        MoonOrbitCircle.rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        MoonOrbitCircle.Draw();

        // Swap the buffers
        glfwSwapBuffers(window);

    }

   

    glfwTerminate();
    return 0;
}



// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}



//// Handles user keyboard input. Supposed to be used every frame, so deltaTime can be calculated appropriately.
//void keyboardInput(GLFWwindow* window, float deltaTime)
//{
//    // Exit
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    // Upwards Rotation
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.Orbit(UP, cameraOrbitRadius, rotateAngle);
//
//    // Downwards Rotation
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.Orbit(DOWN, cameraOrbitRadius, rotateAngle);
//
//    // Rightwards Rotation
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.Orbit(RIGHT, cameraOrbitRadius, rotateAngle);
//
//    // Leftwards Rotation
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.Orbit(LEFT, cameraOrbitRadius, rotateAngle);
//
//
//    // Pause / Start
//    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
//    {
//        if (timeSinceLastToggle > 0.2)
//        {
//            animation = !animation;
//            timeSinceLastToggle = 0.0f;
//        }
//    }

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Sphere()
{
    int prec = 96;
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); } // std::vector::push_back()
    for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); } // inserts new element at
    for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); } // the end of a vector and
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); } // increases the vector size by 1
    // calculate triangle vertices
    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
            float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
        }
    }
    // calculate triangle indices
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }

    std::vector<glm::vec3> vert = vertices;
    std::vector<glm::vec2> tex = texCoords;
    std::vector<glm::vec3> norm = normals;

    std::cout << vertices.size() << std::endl;
    std::cout << vert.size() << std::endl;
    std::cout << tex.size() << std::endl;
    std::cout << norm.size() << std::endl;

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors

   // int numIndices = getNumIndices();
    //for (int i = 0; i < numIndices; i++) {
    //    pvalues.push_back((vert[ind[i]]).x);
    //    pvalues.push_back((vert[ind[i]]).y);
    //    pvalues.push_back((vert[ind[i]]).z);
    //    tvalues.push_back((tex[ind[i]]).s);
    //    tvalues.push_back((tex[ind[i]]).t);
    //    nvalues.push_back((norm[ind[i]]).x);
    //    nvalues.push_back((norm[ind[i]]).y);
    //    nvalues.push_back((norm[ind[i]]).z);
    //}
}

// accessors
int getNumVertices() { return numVertices; }
int getNumIndices() { return numIndices; }
std::vector<int> getIndices() { return indices; }
std::vector<glm::vec3> getVertices() { return vertices; }
std::vector<glm::vec2> getTexCoords() { return texCoords; }
std::vector<glm::vec3> getNormals() { return normals; }

void SphereVertices()
{
    std::vector<int> ind = getIndices();
    std::vector<glm::vec3> vert = getVertices();
    std::vector<glm::vec2> tex = getTexCoords();
    std::vector<glm::vec3> norm = getNormals();
    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors

    int numIndices = getNumIndices();
    for (int i = 0; i < numIndices; i++) {
        pvalues.push_back((vert[ind[i]]).x);
        pvalues.push_back((vert[ind[i]]).y);
        pvalues.push_back((vert[ind[i]]).z);
        tvalues.push_back((tex[ind[i]]).s);
        tvalues.push_back((tex[ind[i]]).t);
        nvalues.push_back((norm[ind[i]]).x);
        nvalues.push_back((norm[ind[i]]).y);
        nvalues.push_back((norm[ind[i]]).z);
    }

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

    GLuint vao[1];
    GLuint vbo[4];

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(3, vbo);
    // put the vertices into buffer #0
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
    // put the texture coordinates into buffer #1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
    // put the normals into buffer #2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    // put the normals into buffer #2
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * 4, &indices[0], GL_STATIC_DRAW);


    GLuint sphereTexture = TextureLoading::LoadTexture("res/images/container2.png");

    Shader shader("res/shaders/cube.vs", "res/shaders/cube.frag");
    shader.Use();

    //// Bind Textures using texture units
    glBindVertexArray(vao[0]);

    // Enable vertex attibute arrays for each vertex attrib
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(0);

    // Bind your VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    // Set vertex attribute pointers to the load correct data. Update here to load the correct attributes.

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)9);



    // If has texture, set up texture unit(s): update here for texture rendering
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sphereTexture);

    // Bind your Element Array
  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    // Render
  //  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glDrawArrays(GL_TRIANGLES, 0, size(indices));
    //glDrawArrays(GL_TRIANGLES, 0, size(Indices));

    // Disable vertex arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}