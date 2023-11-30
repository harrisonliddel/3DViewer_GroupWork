#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::vector<float> vertexData;

// ALL THE VARIABLES
float cameraX = 0.0f; //CAMERAS X AXIS
float cameraY = 0.0f; //CAMERAS Y AXIS
float cameraZ = 1.3f; //CAMERAS Z AXIS

float cameraRotationAngleX = 0.0f; //CAMERAS ROTATION X AXIS
float cameraRotationAngleY = 0.0f; //CAMERAS ROTATION Y AXIS
float cameraRotationAngleZ = 0.0f; //CAMERAS ROTATION Z AXIS

float modelX = 0.0f; //MODEL X AXIS
float modelY = 0.0f; //MODEL Y AXIS
float modelZ = 0.6f; //MODEL Z AXIS

float modelRotationAngleX = 100.0f; //MODEL ROTATION X AXIS
float modelRotationAngleY = 180.0f; //MODEL ROTATION Z AXIS
float modelRotationAngleZ = 450.0f; //MODEL ROTATION Y AXIS

float scaleFactor = 0.007f; //MODELS SCALE FACTOR

GLfloat modelColor[] = {0.0f, 1.0f, 0.0f}; // Initial color (RGB)
GLfloat backgroundColor[] = {0.0f, 0.0f, 0.0f, 1.0f}; // Initial background color (RGB)

// ---------------

void processMesh(const aiMesh* mesh) {
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            unsigned int index = face.mIndices[j];
            vertexData.push_back(mesh->mVertices[index].x);
            vertexData.push_back(mesh->mVertices[index].y);
            vertexData.push_back(mesh->mVertices[index].z);
        }
    }
}

void drawScene(const aiScene* scene) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the camera
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    // Rotate the camera based on user input
    glRotatef(cameraRotationAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotationAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(cameraRotationAngleZ, 0.0f, 0.0f, 1.0f);

    // Translate and scale the model
    glPushMatrix();
    glTranslatef(modelX, modelY, modelZ);
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Additional rotation around model's X, Y, Z axes
    glRotatef(modelRotationAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(modelRotationAngleY, 0.0f, 1.0f, 0.0f);
    glRotatef(modelRotationAngleZ, 0.0f, 0.0f, 1.0f);

    // Set the model color
    glColor3fv(modelColor);

    // Process each mesh in the scene
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];
        processMesh(mesh);

        // Create VBO and buffer data
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        // Enable vertex array and specify the vertex attributes
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, nullptr);

        // Draw the mesh
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 3);

        // Disable vertex array and clean up
        glDisableClientState(GL_VERTEX_ARRAY);
        glDeleteBuffers(1, &vbo);
        vertexData.clear();
    }

    // Restore the model transformation
    glPopMatrix();
}

// Function to handle keyboard input
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static bool rotateAnimation = false;
    static double lastTime = glfwGetTime();
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {

            // TRANSLATING THE CAMERA
            case GLFW_KEY_K:
                cameraY += 0.1f;
                break;
            case GLFW_KEY_I:
                cameraY -= 0.1f;
                break;
            case GLFW_KEY_L:
                cameraX -= 0.1f;
                break;
            case GLFW_KEY_J:
                cameraX += 0.1f;
                break;
            case GLFW_KEY_U:
                cameraZ += 0.1f;
                break;
            case GLFW_KEY_O:
                cameraZ -= 0.1f;
                break;

                // TRANSLATING THE MODEL
            case GLFW_KEY_A:
                modelY += 0.1f;
                break;
            case GLFW_KEY_D:
                modelY -= 0.1f;
                break;
            case GLFW_KEY_S:
                modelX -= 0.1f;
                break;
            case GLFW_KEY_W:
                modelX += 0.1f;
                break;

                // ROTATING THE CAMERA
            case GLFW_KEY_B:
                // Rotate the camera around the X-axis
                cameraRotationAngleX += 10.0f;
                break;
            case GLFW_KEY_N:
                // Rotate the camera around the Z-axis
                cameraRotationAngleZ += 10.0f;
                break;
            case GLFW_KEY_M:
                // Rotate the camera around the Y-axis
                cameraRotationAngleY += 10.0f;
                break;

                // ROTATING THE MODEL
            case GLFW_KEY_E:
                // Rotate the model around its X-axis
                modelRotationAngleX += 10.0f;
                break;
            case GLFW_KEY_Q:
                // Rotate the model around its X-axis in the opposite direction
                modelRotationAngleX -= 10.0f;
                break;
            case GLFW_KEY_Z:
                // Rotate the model around its Y-axis
                modelRotationAngleY += 10.0f;
                break;
            case GLFW_KEY_X:
                // Rotate the model around its Y-axis in the opposite direction
                modelRotationAngleY -= 10.0f;
                break;
            case GLFW_KEY_C:
                // Rotate the model around its Z-axis
                modelRotationAngleZ += 10.0f;
                break;
            case GLFW_KEY_V:
                // Rotate the model around its Z-axis in the opposite direction
                modelRotationAngleZ -= 10.0f;
                break;

                // SCALING THE MODEL
            case GLFW_KEY_EQUAL:
                // Increase the scale factor
                scaleFactor += 0.001f;
                break;
            case GLFW_KEY_MINUS:
                // Decrease the scale factor, but ensure it doesn't go below a minimum value
                scaleFactor = std::max(0.001f, scaleFactor - 0.001f);
                break;
        }

        if (rotateAnimation) {
            // Update rotation angles based on elapsed time
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastTime;

            // Adjust these values based on your desired animation speed
            const float rotationSpeedX = 0.0f;
            const float rotationSpeedY = 500.0f;
            const float rotationSpeedZ = 0.0f;

            cameraRotationAngleX += rotationSpeedX * static_cast<float>(deltaTime);
            cameraRotationAngleY += rotationSpeedY * static_cast<float>(deltaTime);
            cameraRotationAngleZ += rotationSpeedZ * static_cast<float>(deltaTime);

            lastTime = currentTime;

            std::cout << "Animation in progress...\n";

            glfwPostEmptyEvent();
        }

        std::cout << "Camera Position: X=" << cameraX << ", Y=" << cameraY << ", Z=" << cameraZ << std::endl;
        std::cout << "Camera Rotation Angles: X=" << cameraRotationAngleX << ", Y=" << cameraRotationAngleY
                  << ", Z=" << cameraRotationAngleZ << std::endl;
        std::cout << "Model Position: X=" << modelX << ", Y=" << modelY << ", Z=" << modelZ << std::endl;
        std::cout << "Model Scale: " << scaleFactor << std::endl;
        std::cout << "Model Rotation Angles: X=" << modelRotationAngleX << ", Y=" << modelRotationAngleY
                  << ", Z=" << modelRotationAngleZ << std::endl;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set GLFW error callback
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error: " << description << std::endl;
    });

    // Create a GLFW windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "STL Viewer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Load STL file using Assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("/Users/harrison/Library/CloudStorage/OneDrive-LiverpoolHopeUniversity/Uni Work/3D Vision/Coursework/3D Viewer/robo.STL", aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading STL file: " << importer.GetErrorString() << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the keyboard callback
    glfwSetKeyCallback(window, keyboard);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        // Draw the scene
        drawScene(scene);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
