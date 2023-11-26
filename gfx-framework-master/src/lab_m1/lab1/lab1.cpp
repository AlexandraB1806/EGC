#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

string currName = "sphere";
int idx = 1;
glm::vec3 currScale = glm::vec3(0.7f);


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    red = 0;
    green = 0;
    blue = 0;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("archer");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters\\archer"), "Archer.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation\\bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    position = glm::vec3(3, 0, 0); // for archer
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(red, green, blue, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(x, y, z), glm::vec3(0.5f)); // Task5

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    // Bonus
    // archer
    glClearColor(0, 0, 0, 1);
    RenderMesh(meshes["archer"], position, glm::vec3(0.01f));

    // 1 - sphere
    glClearColor(0, 0, 0, 1);
    RenderMesh(meshes["sphere"], position, glm::vec3(0.7f)); // Task5

    // 2 - teapot
    glClearColor(0, 0, 0, 1);
    RenderMesh(meshes["teapot"], position, glm::vec3(0.01f));

    // 3 - bamboo
    glClearColor(0, 0, 0, 1);
    RenderMesh(meshes["bamboo"], position, glm::vec3(0.1f));

    // Task4
    glClearColor(0, 0, 0, 1);
    RenderMesh(meshes[currName], glm::vec3(5, 0, 0), glm::vec3(0.5f));
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // viteza cu care ne miscam intr-o secunda
    float speed = 1;
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    
    // Bonus
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_W)) {
            position.z += speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_A)) {
            position.x -= speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            position.z -= speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            position.x += speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_E)) {
            position.y += speed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_Q)) {
            position.y -= speed * deltaTime;
        }
    }

    //// Task5
    //// front
    //if (window->KeyHold(GLFW_KEY_W)) {
    //    z = z + 0.1;
    //}

    //// left
    //if (window->KeyHold(GLFW_KEY_A)) {
    //    x = x - 0.1;
    //}

    //// behind
    //if (window->KeyHold(GLFW_KEY_S)) {
    //    z = z - 0.1;
    //}

    //// right
    //if (window->KeyHold(GLFW_KEY_D)) {
    //    x = x + 0.1;
    //}

    //// up
    //if (window->KeyHold(GLFW_KEY_E)) {
    //    y = y + 0.1;
    //}

    //// down
    //if (window->KeyHold(GLFW_KEY_Q)) {
    //    y = y - 0.1;
    //}
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        red += 0.2;
       // green += 0.3;
       // blue += 0.1;
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    // Task4
    if (key == GLFW_KEY_B) {
        currName = name[idx % 3];
        currScale = scale[idx % 3];
        idx++;
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}