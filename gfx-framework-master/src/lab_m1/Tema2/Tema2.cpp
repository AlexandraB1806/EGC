#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/object3D.h"

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

float FoV = glm::radians(85.0f);

void Tema2::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 4.f, resolution.y / 4.f);

    float width = window->props.aspectRatio;

    // Mut camera, si dupa mut obiectul dupa camera
    renderCameraTarget = false;

    camera = new implemented::CameraTema2();

    // Am ales un punct de start de pe pista
    tx = -2.17f;
    tz = 8.29f;

    carCenter = glm::vec3(tx, 0.2f, tz);

    camera->Set(carCenter + glm::vec3(-camera->distanceToTarget, 0.5f, 0), carCenter, glm::vec3(0, 1, 0));

    // Resetare tx si ty in functie de pozitia masinii
    tx = camera->GetTargetPosition().x;
    tz = camera->GetTargetPosition().z;

    angle = 0;

    // -------------------------------- DEFINIRE OBIECTE ----------------------------------------------

    // Copacel
    Mesh* tree = object3D::CreateTree("tree");
    meshes["tree"] = tree;

    // Masina jucatorului - roz :)
    Mesh* car = object3D::CreateCar("car", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(1, 0.078f, 0.576f));
    meshes["car"] = car;

    // Gazon
    grass = object3D::CreateGrass(glm::vec3(-25, -0.01f, 25), glm::vec3(25, -0.01f, 25),
                                  glm::vec3(25, -0.01f, -25), glm::vec3(-25, -0.01f, -25));

    // Pista
    Mesh* track = object3D::CreateTrack("track", outerPoints, innerPoints, moreVertices, innerTree, outerTree);
    meshes["track"] = track;

    // ------------------------------------- TRASEU ADVERSARI --------------------------------------------
    // Masina adversar 1 - rosie
    Mesh* enemy1 = object3D::CreateCar("enemy1", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(1, 0, 0));
    meshes["enemy1"] = enemy1;

    // Masina adversar 2 - albastra
    Mesh* enemy2 = object3D::CreateCar("enemy2", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(0, 0, 1));
    meshes["enemy2"] = enemy2;

    // Masina adversar 3 - galbena
    Mesh* enemy3 = object3D::CreateCar("enemy3", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(1, 1, 0));
    meshes["enemy3"] = enemy3;

    // Masina adversar 4 - turcoaz
    Mesh* enemy4 = object3D::CreateCar("enemy4", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(0, 1, 1));
    meshes["enemy4"] = enemy4;

    // Masina adversar 5 - mov
    Mesh* enemy5 = object3D::CreateCar("enemy5", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(0.541f, 0.168f, 0.886f));
    meshes["enemy5"] = enemy5;

    // Masina adversar 6 - portocaliu
    Mesh* enemy6 = object3D::CreateCar("enemy6", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(1, 0.549f, 0));
    meshes["enemy6"] = enemy6;

    // Masina adversar 7 - green spring
    Mesh* enemy7 = object3D::CreateCar("enemy7", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(0, 1, 0.498f));
    meshes["enemy7"] = enemy7;

    // Masina adversar 8 - thistle
    Mesh* enemy8 = object3D::CreateCar("enemy8", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(0.847f, 0.749f, 0.866f));
    meshes["enemy8"] = enemy8;

    // Masina adversar 9 - steelblue
    Mesh* enemy9 = object3D::CreateCar("enemy9", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(0.274f, 0.509f, 0.705f));
    meshes["enemy9"] = enemy9;

    // Masina adversar 10 - yellow green
    Mesh* enemy10 = object3D::CreateCar("enemy10", glm::vec3(0, 0, 0), 0.3f, 0.1f, 0.1f, glm::vec3(0.678f, 1, 0.184f));
    meshes["enemy10"] = enemy10;

    GenerateEnemyTrajectory(moreVertices);

    // Program pentru shader
    {
        Shader* shader = new Shader("Tema2Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void Tema2::GenerateEnemyTrajectory(std::vector<VertexFormat> points)
{
    glm::vec3 D, P, R, A;
    glm::vec3 UP = glm::vec3(0, 1, 0);

    float dist1 = 0.2f;
    float dist2 = -0.3f;
    float dist3 = 0;
    float dist4 = 0.1f;
    float dist5 = 0.3f;
    float dist6 = -0.2f;
    float dist7 = -0.1f;
    float dist8 = 0.25f;
    float dist9 = 0.15f;
    float dist10 = -0.15f;

    for (int i = 0; i < points.size() - 1; i++) {
        D = points[i + 1].position - points[i].position;
        P = glm::normalize(glm::cross(D, UP));

        R = points[i].position + dist1 * P;
        verticesEnemy1.push_back(VertexFormat(R));

        R = points[i].position + dist2 * P;
        verticesEnemy2.push_back(VertexFormat(R));

        R = points[i].position + dist3 * P;
        verticesEnemy3.push_back(VertexFormat(R));

        R = points[i].position + dist4 * P;
        verticesEnemy4.push_back(VertexFormat(R));

        R = points[i].position + dist5 * P;
        verticesEnemy5.push_back(VertexFormat(R));

        R = points[i].position + dist6 * P;
        verticesEnemy6.push_back(VertexFormat(R));

        R = points[i].position + dist7 * P;
        verticesEnemy7.push_back(VertexFormat(R));

        R = points[i].position + dist8 * P;
        verticesEnemy8.push_back(VertexFormat(R));

        R = points[i].position + dist9 * P;
        verticesEnemy9.push_back(VertexFormat(R));

        R = points[i].position + dist10 * P;
        verticesEnemy10.push_back(VertexFormat(R));
    }

    id1 = verticesEnemy1.size() - 500;
    id2 = verticesEnemy2.size() - 1000;
    id3 = verticesEnemy3.size() - 2000;
    id4 = verticesEnemy4.size() - 3000;
    id5 = verticesEnemy5.size() - 4000;
    id6 = verticesEnemy6.size() - 5000;
    id7 = verticesEnemy7.size() - 5500;
    id8 = verticesEnemy8.size() - 6000;
    id9 = verticesEnemy9.size() - 10;
    id10 = verticesEnemy10.size() - 7000;
}


void Tema2::FrameStart()
{
    glClearColor(0.529f, 0.807f, 0.980f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::RenderScene()
{
    // Pista
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
    RenderMesh(meshes["track"], shaders["Tema2Shader"], modelMatrix);

    // Gazon
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
    for (int i = 0; i < grass.size(); i++) {
        RenderMesh(grass[i], shaders["Tema2Shader"], modelMatrix);
    }

    // Masina jucator
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(tx, 0.05f, tz));
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    RenderMesh(meshes["car"], shaders["Tema2Shader"], modelMatrix);
   

    // --------------------------- COPACI -----------------------------------
    for (int i = 0; i < outerTree.size(); i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, outerTree[i]);
        RenderMesh(meshes["tree"], shaders["Tema2Shader"], modelMatrix);
    }

    for (int i = 0; i < innerTree.size(); i++) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, innerTree[i]);
        RenderMesh(meshes["tree"], shaders["Tema2Shader"], modelMatrix);
    }
    
    // --------------------------------------------------------- ADVERSARI ------------------------------------------------------------------------------------

    // 1
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy1[id1].position.x, 0.05f, verticesEnemy1[id1].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id1 + 1 < verticesEnemy1.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy1[id1 + 1].position.x - verticesEnemy1[id1].position.x) / 
                                                    (verticesEnemy1[id1 + 1].position.z - verticesEnemy1[id1].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy1[0].position.x - verticesEnemy1[verticesEnemy1.size() - 1].position.x) / 
                                                (verticesEnemy1[0].position.z - verticesEnemy1[verticesEnemy1.size() - 1].position.z)),
                              glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy1"], shaders["Tema2Shader"], modelMatrix);

    id1 -= 1;
    if (id1 < 0) {
        id1 = verticesEnemy1.size() - 1;
    }

    // 2
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy2[id2].position.x, 0.05f, verticesEnemy2[id2].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id2 + 1 < verticesEnemy2.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy2[id2 + 1].position.x - verticesEnemy2[id2].position.x) /
                                                    (verticesEnemy2[id2 + 1].position.z - verticesEnemy2[id2].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy2[0].position.x - verticesEnemy2[verticesEnemy2.size() - 1].position.x) /
                                                (verticesEnemy2[0].position.z - verticesEnemy2[verticesEnemy2.size() - 1].position.z)),
                              glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy2"], shaders["Tema2Shader"], modelMatrix);

    id2 -= 1;
    if (id2 < 0) {
       id2 = verticesEnemy2.size() - 1;
    }

    // 3
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy3[id3].position.x, 0.05f, verticesEnemy3[id3].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id3 + 1 < verticesEnemy3.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy3[id3 + 1].position.x - verticesEnemy3[id3].position.x) /
            (verticesEnemy3[id3 + 1].position.z - verticesEnemy3[id3].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy3[0].position.x - verticesEnemy3[verticesEnemy3.size() - 1].position.x) /
        (verticesEnemy3[0].position.z - verticesEnemy3[verticesEnemy3.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy3"], shaders["Tema2Shader"], modelMatrix);

    id3 -= 1;
    if (id3 < 0) {
        id3 = verticesEnemy3.size() - 1;
    }

    // 4
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy4[id4].position.x, 0.05f, verticesEnemy4[id4].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id4 + 1 < verticesEnemy4.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy4[id4 + 1].position.x - verticesEnemy4[id4].position.x) /
            (verticesEnemy4[id4 + 1].position.z - verticesEnemy4[id4].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy4[0].position.x - verticesEnemy4[verticesEnemy4.size() - 1].position.x) /
        (verticesEnemy4[0].position.z - verticesEnemy4[verticesEnemy4.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy4"], shaders["Tema2Shader"], modelMatrix);

    id4 -= 1;
    if (id4 < 0) {
        id4 = verticesEnemy4.size() - 1;
    }

    // 5
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy5[id5].position.x, 0.05f, verticesEnemy5[id5].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id5 + 1 < verticesEnemy5.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy5[id5 + 1].position.x - verticesEnemy5[id5].position.x) /
            (verticesEnemy5[id5 + 1].position.z - verticesEnemy5[id5].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy5[0].position.x - verticesEnemy5[verticesEnemy5.size() - 1].position.x) /
        (verticesEnemy5[0].position.z - verticesEnemy5[verticesEnemy5.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy5"], shaders["Tema2Shader"], modelMatrix);

    id5 -= 1;
    if (id5 < 0) {
        id5 = verticesEnemy5.size() - 1;
    }

    // 6
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy6[id6].position.x, 0.05f, verticesEnemy6[id6].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id6 + 1 < verticesEnemy6.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy6[id6 + 1].position.x - verticesEnemy6[id6].position.x) /
            (verticesEnemy6[id6 + 1].position.z - verticesEnemy6[id6].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy6[0].position.x - verticesEnemy6[verticesEnemy6.size() - 1].position.x) /
        (verticesEnemy6[0].position.z - verticesEnemy6[verticesEnemy6.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy6"], shaders["Tema2Shader"], modelMatrix);

    id6 -= 1;
    if (id6 < 0) {
        id6 = verticesEnemy6.size() - 1;
    }

    // 7
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy7[id7].position.x, 0.05f, verticesEnemy7[id7].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id7 + 1 < verticesEnemy7.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy7[id7 + 1].position.x - verticesEnemy7[id7].position.x) /
            (verticesEnemy7[id7 + 1].position.z - verticesEnemy7[id7].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy7[0].position.x - verticesEnemy7[verticesEnemy7.size() - 1].position.x) /
        (verticesEnemy7[0].position.z - verticesEnemy7[verticesEnemy7.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy7"], shaders["Tema2Shader"], modelMatrix);

    id7 -= 1;
    if (id7 < 0) {
        id7 = verticesEnemy7.size() - 1;
    }

    // 8
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy8[id8].position.x, 0.05f, verticesEnemy8[id8].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id8 + 1 < verticesEnemy8.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy8[id8 + 1].position.x - verticesEnemy8[id8].position.x) /
            (verticesEnemy8[id8 + 1].position.z - verticesEnemy8[id8].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy8[0].position.x - verticesEnemy8[verticesEnemy8.size() - 1].position.x) /
        (verticesEnemy8[0].position.z - verticesEnemy8[verticesEnemy8.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy8"], shaders["Tema2Shader"], modelMatrix);

    id8 -= 1;
    if (id8 < 0) {
        id8 = verticesEnemy8.size() - 1;
    }

    // 9
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy9[id9].position.x, 0.05f, verticesEnemy9[id9].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id9 + 1 < verticesEnemy9.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy9[id9 + 1].position.x - verticesEnemy9[id9].position.x) /
            (verticesEnemy9[id9 + 1].position.z - verticesEnemy9[id9].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy9[0].position.x - verticesEnemy9[verticesEnemy9.size() - 1].position.x) /
        (verticesEnemy9[0].position.z - verticesEnemy9[verticesEnemy9.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy9"], shaders["Tema2Shader"], modelMatrix);

    id9 -= 1;
    if (id9 < 0) {
        id9 = verticesEnemy9.size() - 1;
    }

    // 10
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(verticesEnemy10[id10].position.x, 0.05f, verticesEnemy10[id10].position.z));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    if (id10 + 1 < verticesEnemy10.size()) {
        modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy10[id10 + 1].position.x - verticesEnemy10[id10].position.x) /
            (verticesEnemy10[id10 + 1].position.z - verticesEnemy10[id10].position.z)), glm::vec3(0, 1, 0));
    }
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, atan((verticesEnemy10[0].position.x - verticesEnemy10[verticesEnemy10.size() - 1].position.x) /
        (verticesEnemy10[0].position.z - verticesEnemy10[verticesEnemy10.size() - 1].position.z)),
        glm::vec3(0, 1, 0));
    RenderMesh(meshes["enemy10"], shaders["Tema2Shader"], modelMatrix);

    id10 -= 1;
    if (id10 < 0) {
        id10 = verticesEnemy10.size() - 1;
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    // Desenare scena principala
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene();

    // Salvare camera inainte de a o modifica pentru minimap
    // (proiectie ortografica)
    glm::vec3 target_position_copy = camera->GetTargetPosition();
    target_position_copy.y = 0;
    float distance_to_target_copy = camera->distanceToTarget;
    glm::vec3 position_copy = camera->position;
    glm::vec3 forward_copy = camera->forward;
    glm::vec3 right_copy = camera->right;
    glm::vec3 up_copy = camera->up;

    // Desenare minimapa
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    projectionMatrix = glm::ortho(-4.0f, 4.0f, -2.0f, 2.0f, 0.0f, 100.0f);
    camera->Set(target_position_copy + glm::vec3(-1, 40, 1), target_position_copy, glm::vec3(0, 1, 0));
    RenderScene();

    // Recuperare valori
    camera->distanceToTarget = distance_to_target_copy;
    camera->position = position_copy;
    camera->forward = forward_copy;
    camera->right = right_copy;
    camera->up = up_copy;
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int carLocation = glGetUniformLocation(shader->program, "CarPos");
    glUniform3fv(carLocation, 1, glm::value_ptr(glm::vec3(tx, 0, tz)));

    mesh->Render();
}

// --------------------------------------------------------- VERIFICARI DACA MASINA SE AFLA PE PISTA -----------------------------------------------------------------
bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b) {
    if (glm::dot(glm::cross(b - a, p1 - a), glm::cross(b - a, p2 - a)) >= 0) {
        return true;
    }
    return false;
}

bool PointInTriangle(glm::vec3 point, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    if (SameSide(point, a, b, c) && SameSide(point, b, a, c) && SameSide(point, c, a, b)) {
        return true;
    }
    return false;
}

bool Tema2::IsInsideTrack(Mesh* track)
{
    int num_vertices = track->vertices.size() / 2;

    for (int i = 0; i < num_vertices; i++) {
        if (PointInTriangle(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z),
            outerPoints[i], outerPoints[(i + 1) % num_vertices], innerPoints[i])) {
            return true;
        }
        if (PointInTriangle(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z),
            innerPoints[i], innerPoints[(i + 1) % num_vertices], outerPoints[(i + 1) % num_vertices])) {
            return true;
        }
    }

    return false;
}


// ------------------------------- COLIZIUNI OBSTACOLE DINAMICE -------------------------
bool CollissionDetection(glm::vec3 centerPlayer, glm::vec3 centerEnemy) {
    float distance;
    distance = sqrt((centerPlayer.x - centerEnemy.x) * (centerPlayer.x - centerEnemy.x) +
        (centerPlayer.y - centerEnemy.y) * (centerPlayer.y - centerEnemy.y) +
        (centerPlayer.z - centerEnemy.z) * (centerPlayer.z - centerEnemy.z));
    return distance < 0.2f;
}

// ---------------------------------------- CONTROL DIRECTIE MASINA -------------------------------------
void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        if (CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy1[id1].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy2[id2].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy3[id3].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy4[id4].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy5[id5].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy6[id6].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy7[id7].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy8[id8].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy9[id9].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy10[id10].position) == false) {
            // Deplaseaza camera in fata
            camera->MoveForward(deltaTime * cameraSpeed * 1.4f);
            if (IsInsideTrack(meshes["track"]) == true) {
                tx = camera->GetTargetPosition().x;
                tz = camera->GetTargetPosition().z;
            }
            else {
                camera->MoveForward(-deltaTime * cameraSpeed * 1.4f);
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // Deplaseaza camera spre stanga
        angle += deltaTime * 0.9f;
        // Rotire third-person in jurul axei OY
        camera->RotateThirdPerson_OY(deltaTime * 0.9f);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy1[id1].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy2[id2].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy3[id3].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy4[id4].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy5[id5].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy6[id6].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy7[id7].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy8[id8].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy9[id9].position) == false &&
            CollissionDetection(glm::vec3(camera->GetTargetPosition().x, 0, camera->GetTargetPosition().z), verticesEnemy10[id10].position) == false) {
            // Deplaseaza camera in spate
            camera->MoveForward(-deltaTime * cameraSpeed * 1.4f);
            if (IsInsideTrack(meshes["track"]) == true) {
                tx = camera->GetTargetPosition().x;
                tz = camera->GetTargetPosition().z;
            }
            else {
                camera->MoveForward(deltaTime * cameraSpeed * 1.4f);
            }
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // Deplaseaza camera spre dreapta
        angle -= deltaTime * 0.9f;
        // Rotire third-person in jurul axei OY
        camera->RotateThirdPerson_OY(-deltaTime * 0.9f);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
