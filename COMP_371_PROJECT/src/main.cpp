#include <iostream>
#include <vector>
#include <string>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL


// IMPORTANT 1
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs
#include "learnopengl/stb_image.h"
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION


#include "learnopengl/shader_m.h"
#include "learnopengl/camera.h"
#include "learnopengl/model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces);






// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(5.0f, 5.0f,10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float pi = std::acos(-1.0f);



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "footballPitch", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }


    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    


    // build and compile shaders
    // -------------------------
    Shader ourShader("../src/1.vertexShader.vs", "../src/1.fragmentShader.fs");
    Shader lightShader("../shaders/phong.vert.glsl", "../shaders/phong.frag.glsl");
    Shader cloudboxShader("../src/cloudbox.vs", "../src/cloudbox.fs");

    // load models
    // -----------
    Model ourModel("../src/miniStadium/miniStadium.obj");
   


    float cloudboxVertices[] = {
        // positions
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


    unsigned int VBO, cloudboxVAO;
    glGenVertexArrays(1, &cloudboxVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(cloudboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cloudboxVertices), cloudboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<std::string> faces
    {
        "../cloudbox/bluecloud_rt.jpg",
        "../cloudbox/bluecloud_lf.jpg",
        "../cloudbox/bluecloud_up.jpg",
        "../cloudbox/bluecloud_dn.jpg",
        "../cloudbox/bluecloud_ft.jpg",
        "../cloudbox/bluecloud_bk.jpg"
    };

    glm::vec3 lightPos{5.0f, 1.0f, 0.0f};
    glm::vec3 cubeLightPosition(40.0f, 20.0f, 5.0f); // Adjust position as needed
    glm::vec3 cubeLightScale(10.0f); // Adjust scale as needed

  float cubeLightVertices[] = {
    // Front face
    -0.1f, -0.1f,  0.1f, 1.0f,
     0.1f, -0.1f,  0.1f, 1.0f,
     0.1f,  0.1f,  0.1f, 1.0f,
    -0.1f,  0.1f,  0.1f, 1.0f,

    // Back face
    -0.1f, -0.1f, -0.1f, 1.0f,
     0.1f, -0.1f, -0.1f, 1.0f,
     0.1f,  0.1f, -0.1f, 1.0f,
    -0.1f,  0.1f, -0.1f, 1.0f,

    // Top face
    -0.1f,  0.1f,  0.1f, 1.0f,
     0.1f,  0.1f,  0.1f, 1.0f,
     0.1f,  0.1f, -0.1f, 1.0f,
    -0.1f,  0.1f, -0.1f, 1.0f,

    // Bottom face
    -0.1f, -0.1f,  0.1f, 1.0f,
     0.1f, -0.1f,  0.1f, 1.0f,
     0.1f, -0.1f, -0.1f, 1.0f,
    -0.1f, -0.1f, -0.1f, 1.0f,

    // Left face
    -0.1f, -0.1f, -0.1f, 1.0f,
    -0.1f, -0.1f,  0.1f, 1.0f,
    -0.1f,  0.1f,  0.1f, 1.0f,
    -0.1f,  0.1f, -0.1f, 1.0f,

    // Right face
     0.1f, -0.1f, -0.1f, 1.0f,
     0.1f, -0.1f,  0.1f, 1.0f,
     0.1f,  0.1f,  0.1f, 1.0f,
     0.1f,  0.1f, -0.1f, 1.0f,
};

unsigned int cubeLightIndices[] = {
    0, 1, 2, 2, 3, 0,        // Front face
    4, 5, 6, 6, 7, 4,        // Back face
    8, 9, 10, 10, 11, 8,     // Top face
    12, 13, 14, 14, 15, 12,  // Bottom face
    16, 17, 18, 18, 19, 16,  // Left face
    20, 21, 22, 22, 23, 20   // Right face
};


    unsigned int cubeLightVAO, cubeLightVBO, cubeLightEBO;
    glGenVertexArrays(1, &cubeLightVAO);
    glGenBuffers(1, &cubeLightVBO);
    glGenBuffers(1, &cubeLightEBO);

    glBindVertexArray(cubeLightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeLightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeLightVertices), cubeLightVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeLightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeLightIndices), cubeLightIndices, GL_STATIC_DRAW);
    // Set vertex attribute pointers
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);


    unsigned int cubemapTexture = loadCubemap(faces);

    cloudboxShader.use();
    cloudboxShader.setInt("cloudbox", 0);
    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ourShader.use();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    ourShader.setFloat("material.shininess", 32.0f);
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        ourShader.setVec3("light.position", cubeLightPosition);
        ourShader.setVec3("viewPos", camera.Position);

        // light properties
        // light properties
        ourShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f); // increase ambient intensity
        ourShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f); // increase diffuse intensity
        ourShader.setVec3("light.specular", 2.0f, 2.0f, 2.0f); // increase specular intensity


        // material properties
        ourShader.setFloat("material.specular", 64.0f);



        // don't forget to enable shader before setting uniforms
        ourShader.use();
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //render the loaded model with assimp model loader
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);


        
        // Render cube light
        lightShader.use();
        glm::mat4 cubemodel = glm::mat4(1.0f);
        cubemodel = glm::translate(cubemodel, cubeLightPosition);
        cubemodel = glm::scale(cubemodel, cubeLightScale);
        lightShader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
        lightShader.setMat4("view", camera.GetViewMatrix());
        lightShader.setMat4("model", cubemodel);
        glBindVertexArray(cubeLightVAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);      

        //draw cloudbox last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        cloudboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        cloudboxShader.setMat4("view", view);
        cloudboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(cloudboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
