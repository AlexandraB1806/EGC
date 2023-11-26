#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    // old
    /*angleJump = 0.f;
    startPos = glm::vec3(0, 0, 0);
    endPos = glm::vec3(JUMP_DISTANCE, 0, 0);
    */

    // BONUS
    translateStepX = 0.05;
    translateStepY = 0.05;
    translateXSun = 0;
    translateYSun = 0;

    angularStep = 0;

    rEarth = 4;
    rMoon = 2;
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab4::RenderScene() {
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    /*modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(midPos[0], midPos[1], midPos[2]);
    modelMatrix *= transform3D::RotateOZ(angleJump);
    modelMatrix *= transform3D::Translate(startPos[0] - midPos[0], startPos[1] - midPos[1], startPos[2] - midPos[2]);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);*/

    ////Soare
    //modelMatrix = glm::mat4(1);
    //modelMatrix *= transform3D::Translate(5, 4, 0);
    //modelMatrix *= transform3D::Translate(translateXSun, translateYSun, 0);
    //modelMatrix *= transform3D::RotateOY(angularStep);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    ////Pamant
    //modelMatrix = glm::mat4(1);
    //modelMatrix *= transform3D::Translate(-2.5f, 2.5f, -1.5f);
    //modelMatrix *= transform3D::Translate(translateXSun, translateYSun, 0);
    ////modelMatrix *= transform3D::RotateOY(angularStep);
    //modelMatrix *= transform3D::RotateOY(angularStep * 2);
    //modelMatrix *= transform3D::Translate(rEarth, 0, 0);
    ////modelMatrix *= transform3D::RotateOY(angularStep * 2);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    ///*modelMatrix *= transform3D::Scale(0.75, 0.75, 0.75);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);*/

    ////Luna
    //modelMatrix = glm::mat4(1);
    ////Rotatie in jurul Pamantului
    //modelMatrix *= transform3D::RotateOY(angularStep * 4);
    //modelMatrix *= transform3D::Translate(rMoon, 0, 0);
    ////Rotatie in jurul axei
    ////modelMatrix *= transform3D::RotateOY(angularStep * 4);
    ////modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    //RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    RenderScene();
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    RenderScene();

    DrawCoordinateSystem();

    // old
    //{
    //    midPos = (startPos + endPos) * 0.5f;

    //    /*modelMatrix = glm::mat4(1);
    //    modelMatrix *= transform3D::Translate(midPos[0], midPos[1], midPos[2]);
    //    modelMatrix *= transform3D::RotateOZ(angleJump);
    //    modelMatrix *= transform3D::Translate(startPos[0] - midPos[0], startPos[1] - midPos[1], startPos[2] - midPos[2]);

    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);*/
    //}

    // BONUS
    if (translateXSun >= 18) {
        translateStepX = -0.05;
        translateStepY = -0.05;
    }
    else if (translateXSun <= -15) {
        translateStepX = 0.05;
        translateStepY = 0.05;
    }
    translateXSun += translateStepX * 20 * deltaTimeSeconds;
    translateYSun += translateStepY * 20 * deltaTimeSeconds;

    //Soare
  /*  modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 2.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateXSun, translateYSun, 0);
    modelMatrix *= transform3D::RotateOY(angularStep);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);*/
    //Coordonate Soare
    /*modelMatrix *= transform3D::Translate(-2.5f, 2.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateXSun, translateYSun, 0);
    modelMatrix *= transform3D::RotateOY(angularStep);*/
    //  Rotatie in jurul axei
    //modelMatrix *= transform3D::RotateOY(angularStep * 2);

   // //Pamant
   // modelMatrix = glm::mat4(1);
   // //Coordonate Pamant 
   // modelMatrix *= transform3D::Translate(-2.5f, 2.5f, -1.5f);
   // modelMatrix *= transform3D::Translate(translateXSun, translateYSun, 0);
   // modelMatrix *= transform3D::RotateOY(angularStep);
   // modelMatrix *= transform3D::RotateOY(angularStep * 2);
   // modelMatrix *= transform3D::Translate(rEarth, 0, 0);
   // modelMatrix *= transform3D::RotateOY(angularStep * 2);

   // //Rotatie in jurul soarelui
   //// modelMatrix *= transform3D::RotateOY(angularStep * 2);
   //// modelMatrix *= transform3D::Translate(rEarth, 0, 0);

   // modelMatrix *= transform3D::Scale(0.75, 0.75, 0.75);
   // RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

   // //Luna
   // modelMatrix = glm::mat4(1);
   // //Rotatie in jurul Pamantului
   // modelMatrix *= transform3D::RotateOY(angularStep * 4);
   // modelMatrix *= transform3D::Translate(rMoon, 0, 0);

   // //Rotatie in jurul axei
   // modelMatrix *= transform3D::RotateOY(angularStep * 4);

   // modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
   // RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    float speed = 1;

    // FIRST CUBE
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        // front
        if (window->KeyHold(GLFW_KEY_W)) {
            translateZ += speed * deltaTime;
        }

        // left
        if (window->KeyHold(GLFW_KEY_A)) {
            translateX -= speed * deltaTime;
        }

        // back
        if (window->KeyHold(GLFW_KEY_R)) {
            translateZ -= speed * deltaTime;
        }

        // right
        if (window->KeyHold(GLFW_KEY_S)) {
            translateX += speed * deltaTime;
        }

        // down
        if (window->KeyHold(GLFW_KEY_D)) {
            translateY -= speed * deltaTime;
        }

        // up
        if (window->KeyHold(GLFW_KEY_F)) {
            translateY += speed * deltaTime;
        }

        // MAP
        // up
        if (window->KeyHold(GLFW_KEY_I)) {
            miniViewportArea.y += speed;
        }

        // left
        if (window->KeyHold(GLFW_KEY_J)) {
            miniViewportArea.x -= speed;
        }

        // down
        if (window->KeyHold(GLFW_KEY_K)) {
            miniViewportArea.y -= speed;
        }

        // right
        if (window->KeyHold(GLFW_KEY_L)) {
            miniViewportArea.x += speed;
        }

    }

    // SECOND CUBE
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += speed * deltaTime;
        scaleY += speed * deltaTime;
        scaleZ += speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= speed * deltaTime;
        scaleY -= speed * deltaTime;
        scaleZ -= speed * deltaTime;
    }

    // THIRD CUBE
    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += 3 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= 3 * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += 3 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= 3 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += 3 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= 3 * deltaTime;
    }

   
    //// BONUS: Make the cube jump
    //if (window->KeyHold(GLFW_KEY_P)) {
    //    angleJump -= deltaTime;

    //    if (angleJump <= -M_PI) {
    //        startPos = endPos;
    //        endPos[0] += JUMP_DISTANCE;
    //        angleJump = 0.f;
    //    }
    //}
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    
    // TODO(student): Add viewport movement and scaling logic
    float speed = 10;

    //// MAP
    //// up
    //if (window->KeyHold(GLFW_KEY_I)) {
    //    miniViewportArea.y += speed;
    //}

    //// left
    //if (window->KeyHold(GLFW_KEY_J)) {
    //    miniViewportArea.x -= speed;
    //}

    //// down
    //if (window->KeyHold(GLFW_KEY_K)) {
    //    miniViewportArea.y -= speed;
    //}

    //// right
    //if (window->KeyHold(GLFW_KEY_L)) {
    //    miniViewportArea.x += speed;
    //}

    // smaller
    if (window->KeyHold(GLFW_KEY_U)) {
        miniViewportArea.width -= speed;
        miniViewportArea.height -= speed;
    }

    // bigger
    if (window->KeyHold(GLFW_KEY_O)) {
        miniViewportArea.width += speed;
        miniViewportArea.height += speed;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
