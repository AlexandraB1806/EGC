#include "lab_m1/Tema2/object3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object3D::CreateCar(
    const std::string& name,
    glm::vec3 center,
    float length,
    float height,
    float width,
    glm::vec3 color)
{
    Mesh* carObj = new Mesh(name);

    std::vector<VertexFormat> vertices
    {
        VertexFormat(center + glm::vec3(-length / 2, -height / 2, width / 2), color),
        VertexFormat(center + glm::vec3(length / 2, -height / 2, width / 2), color),
        VertexFormat(center + glm::vec3(-length / 2, height / 2, width / 2), color),
        VertexFormat(center + glm::vec3(length / 2, height / 2, width / 2), color),
        VertexFormat(center + glm::vec3(-length / 2, -height / 2, -width / 2), color),
        VertexFormat(center + glm::vec3(length / 2, -height / 2, -width / 2), color),
        VertexFormat(center + glm::vec3(-length / 2, height / 2, -width / 2), color),
        VertexFormat(center + glm::vec3(length / 2, height / 2, -width / 2), color)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4
    };

    carObj->InitFromData(vertices, indices);
    return carObj;
}

Mesh* object3D::CreateTree(
    const std::string& name)
{
    Mesh* treeObj = new Mesh(name);

    std::vector<VertexFormat> vertices
    {
        // Trunchi copacel
        VertexFormat(glm::vec3(0, -0.01f, 0.1f), glm::vec3(0.36f, 0.25f, 0.2f)),
        VertexFormat(glm::vec3(0.1f, -0.01f, 0.1f), glm::vec3(0.36f, 0.25f, 0.2f)),
        VertexFormat(glm::vec3(0, 0.2f, 0.1f), glm::vec3(0.36f, 0.25f, 0.2f)),
        VertexFormat(glm::vec3(0.1f, 0.2f, 0.1f), glm::vec3(0.36f, 0.25f, 0.2f)),
        VertexFormat(glm::vec3(0, -0.01f, 0), glm::vec3(0.36f, 0.25f, 0.2f)),
        VertexFormat(glm::vec3(0.1f, -0.01f, 0), glm::vec3(0.36f, 0.25f, 0.2f)),
        VertexFormat(glm::vec3(0, 0.2f, 0), glm::vec3(0.36f, 0.25f, 0.2f)),
        VertexFormat(glm::vec3(0.1f, 0.2f, 0), glm::vec3(0.36f, 0.25f, 0.2f)),

        // Coroana copacel
        VertexFormat(glm::vec3(-0.1f, 0.2f, 0.2f), glm::vec3(0, 0.152f, 0)),
        VertexFormat(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0, 0.152f, 0)),
        VertexFormat(glm::vec3(-0.1f, 0.4f, 0.2f), glm::vec3(0, 0.152f, 0)),
        VertexFormat(glm::vec3(0.2f, 0.4f, 0.2f), glm::vec3(0, 0.152f, 0)),
        VertexFormat(glm::vec3(-0.1f, 0.2f, -0.1f), glm::vec3(0, 0.152f, 0)),
        VertexFormat(glm::vec3(0.2f, 0.2f, -0.1f), glm::vec3(0, 0.152f, 0)),
        VertexFormat(glm::vec3(-0.1f, 0.4f, -0.1f), glm::vec3(0, 0.152f, 0)),
        VertexFormat(glm::vec3(0.2f, 0.4f, -0.1f), glm::vec3(0, 0.152f, 0)),
    };

    std::vector<unsigned int> indices =
    {
        // Trunchi copacel
        0, 1, 2,
        1, 3, 2,
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4,

        // Coroana copacel
        8, 9, 10,
        9, 11, 10,
        10, 11, 15,
        10, 15, 14,
        9, 15, 11,
        9, 13, 15,
        14, 15, 12,
        15, 13, 12,
        8, 12, 9,
        9, 12, 13,
        10, 14, 12,
        8, 10, 12
    };

    treeObj->InitFromData(vertices, indices);
    return treeObj;
}

std::vector<Mesh*> object3D::CreateGrass(glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4)
{
    glm::vec3 color = glm::vec3(0.235f, 0.701f, 0.443f);

    std::vector<Mesh*> final_mesh;

    glm::vec3 new_corner1 = corner1;
    glm::vec3 new_corner2 = corner2;
    glm::vec3 new_corner3;
    glm::vec3 new_corner4;

    glm::vec3 new_point_up, new_point_down;

    for (float i = 0.01f; i <= 0.99f; i = i + 0.01f) {
        // Pentru fiecare nou mesh creat intre cele 4 colturi, defines
        // separat vertecsii si indecsii
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        // Creez prin interpolare punctele ce definesc liniile matricei pentru gazon
        new_corner4 = glm::mix(corner1, corner4, i);
        new_corner3 = glm::mix(corner2, corner3, i);

        for (float j = 0.01f; j <= 0.99f; j = j + 0.01f) {
            // Creez prin interpolare punctele ce definesc coloanele matricei pentru gazon
            new_point_up = glm::mix(new_corner1, new_corner2, j);
            new_point_down = glm::mix(new_corner4, new_corner3, j);

            vertices.push_back(VertexFormat(new_point_up, color));
            vertices.push_back(VertexFormat(new_point_down, color));
        }

        // Indicii
        for (int i = 0; i < vertices.size(); i++) {
            indices.push_back(i);
        }

        // Creare mesh-uri partiale pentru mesh-ul final
        Mesh* chunk_mesh = new Mesh("grass");
        chunk_mesh->InitFromData(vertices, indices);
        chunk_mesh->SetDrawMode(GL_TRIANGLE_STRIP);

        final_mesh.push_back(chunk_mesh);

        // Actualizare colturi
        new_corner1 = new_corner4;
        new_corner2 = new_corner3;
    }

    return final_mesh;
}

Mesh* object3D::CreateTrack(
    const std::string& name,
    std::vector<glm::vec3>& outer,
    std::vector<glm::vec3>& inner,
    std::vector<VertexFormat>& moreVertices,
    std::vector<glm::vec3>& innerTree,
    std::vector<glm::vec3>& outerTree)
{
    std::vector<VertexFormat> vertices
    {
        // I7 - A7
        VertexFormat(glm::vec3(-0.36f, 0, 7.99f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I7
        VertexFormat(glm::vec3(0.49f, 0, 8.19f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H7
        VertexFormat(glm::vec3(1.19f, 0, 8.84f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // G7
        VertexFormat(glm::vec3(2.1f, 0, 9.44f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // F7
        VertexFormat(glm::vec3(2.9f, 0, 10.09f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // E7
        VertexFormat(glm::vec3(3.6f, 0, 10.75f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // D7
        VertexFormat(glm::vec3(4.3f, 0, 11.35f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // C7
        VertexFormat(glm::vec3(5.06f, 0, 11.9f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // B7
        VertexFormat(glm::vec3(5.71f, 0, 12.5f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // A7

        // Z6 - A6
        VertexFormat(glm::vec3(6.31f, 0, 13), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Z6
        VertexFormat(glm::vec3(6.96f, 0, 13.56f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // W6
        VertexFormat(glm::vec3(7.82f, 0, 14.16f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // V6
        VertexFormat(glm::vec3(8.52f, 0, 14.71f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // U6
        VertexFormat(glm::vec3(9.27f, 0, 15.36f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // T6
        VertexFormat(glm::vec3(10.22f, 0, 16.01f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // S6
        VertexFormat(glm::vec3(11.08f, 0, 16.62f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // R6
        VertexFormat(glm::vec3(11.88f, 0, 17.27f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Q6
        VertexFormat(glm::vec3(12.78f, 0, 17.62f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // P6
        VertexFormat(glm::vec3(13.79f, 0, 17.82f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // O6
        VertexFormat(glm::vec3(14.75f, 0, 17.45f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // N6
        VertexFormat(glm::vec3(15.49f, 0, 16.82f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // M6
        VertexFormat(glm::vec3(16.03f, 0, 16.01f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // L6
        VertexFormat(glm::vec3(16.55f, 0, 15.06f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // K6
        VertexFormat(glm::vec3(16.35f, 0, 13.86f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // J6
        VertexFormat(glm::vec3(16.1f, 0, 12.6f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I6
        VertexFormat(glm::vec3(15.6f, 0, 11.16f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H6

        // Z5 - A5
        VertexFormat(glm::vec3(15.05f, 0, 9.12f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // V5
        VertexFormat(glm::vec3(15.1f, 0, 7.4f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // U5
        VertexFormat(glm::vec3(15.38f, 0, 5.8f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // T5
        VertexFormat(glm::vec3(16.32f, 0, 4.64f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // S5
        VertexFormat(glm::vec3(17.09f, 0, 3.7f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // R5
        VertexFormat(glm::vec3(17.97f, 0, 2.65f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Q5
        VertexFormat(glm::vec3(18.91f, 0, 2.01f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // P5
        VertexFormat(glm::vec3(19.71f, 0, 0.76f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // O5
        VertexFormat(glm::vec3(19.91f, 0, -0.8f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // N5
        VertexFormat(glm::vec3(19.91f, 0, -2.1f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // M5
        VertexFormat(glm::vec3(20.01f, 0, -3.56f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // L5
        VertexFormat(glm::vec3(20, 0, -5), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // K5
        VertexFormat(glm::vec3(19.36f, 0, -6.52f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // J5
        VertexFormat(glm::vec3(18.75f, 0, -8.07f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I5
        VertexFormat(glm::vec3(17.7, 0, -9.08f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H5
        VertexFormat(glm::vec3(16.6f, 0, -10.08f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // G5
        VertexFormat(glm::vec3(15.64f, 0, -10.68f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // F5
        VertexFormat(glm::vec3(14.62f, 0, -11.28f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // E5
        VertexFormat(glm::vec3(13.7f, 0, -11.64f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // D5
        VertexFormat(glm::vec3(12.73f, 0, -11.89f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // C5
        VertexFormat(glm::vec3(11.83f, 0, -11.94f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // B5
        VertexFormat(glm::vec3(10.78f, 0, -12.09f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // A5

        // Z4 - A4
        VertexFormat(glm::vec3(10.02f, 0, -12.09f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Z4
        VertexFormat(glm::vec3(9.02f, 0, -11.78f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // W4
        VertexFormat(glm::vec3(8.27f, 0, -11.53f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // V4
        VertexFormat(glm::vec3(7.51f, 0, -10.98f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // U4
        VertexFormat(glm::vec3(6.86f, 0, -10.37f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // T4
        VertexFormat(glm::vec3(6.29f, 0, -9.72f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // S4
        VertexFormat(glm::vec3(5.25f, 0, -9.17f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Q4
        VertexFormat(glm::vec3(4.1f, 0, -8.86f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // P4
        VertexFormat(glm::vec3(3.1f, 0, -8.97f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // N4
        VertexFormat(glm::vec3(2.45f, 0, -9.68f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // M4
        VertexFormat(glm::vec3(1.84f, 0, -10.58f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // L4
        VertexFormat(glm::vec3(1.64f, 0, -11.63f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // K4
        VertexFormat(glm::vec3(1.54f, 0, -12.69f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // J4
        VertexFormat(glm::vec3(1.24f, 0, -13.64f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I4
        VertexFormat(glm::vec3(0.69f, 0, -14.39f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H4
        VertexFormat(glm::vec3(0, 0, -15), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // G4
        VertexFormat(glm::vec3(-1.82f, 0, -15.35f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // F4
        VertexFormat(glm::vec3(-2.97f, 0, -15.4f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // E4
        VertexFormat(glm::vec3(-4.18f, 0, -15.35f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // D4
        VertexFormat(glm::vec3(-5.83f, 0, -15.4f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // C4
        VertexFormat(glm::vec3(-6.84f, 0, -15.35f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // B4
        VertexFormat(glm::vec3(-7.89f, 0, -15.3f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // A4

        // Z3 - A3
        VertexFormat(glm::vec3(-8.94f, 0, -15.2f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Z3
        VertexFormat(glm::vec3(-10, 0, -15), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // W3
        VertexFormat(glm::vec3(-10.85f, 0, -14.9f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // V3
        VertexFormat(glm::vec3(-11.9f, 0, -14.7f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // U3
        VertexFormat(glm::vec3(-12.91f, 0, -14.54f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // T3
        VertexFormat(glm::vec3(-14.01f, 0, -14.39f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // S3
        VertexFormat(glm::vec3(-14.97f, 0, -14.09f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // R3
        VertexFormat(glm::vec3(-16.02f, 0, -13.89f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Q3
        VertexFormat(glm::vec3(-17.07f, 0, -13.69f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // P3
        VertexFormat(glm::vec3(-17.83f, 0, -13.44f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // O3
        VertexFormat(glm::vec3(-18.96f, 0, -12.93f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // N3
        VertexFormat(glm::vec3(-19.82f, 0, -12.46f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // M3
        VertexFormat(glm::vec3(-20.71f, 0, -11.84f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // L3
        VertexFormat(glm::vec3(-21.44f, 0, -11.28f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // K3
        VertexFormat(glm::vec3(-21.91f, 0, -10.59f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // J3
        VertexFormat(glm::vec3(-22.01f, 0, -9.66f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I3
        VertexFormat(glm::vec3(-21.96f, 0, -8.87f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H3
        VertexFormat(glm::vec3(-22, 0, -8), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // G3
        VertexFormat(glm::vec3(-22.01f, 0, -6.88f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // F3
        VertexFormat(glm::vec3(-22, 0, -6), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // E3
        VertexFormat(glm::vec3(-21.96f, 0, -5.25f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // D3
        VertexFormat(glm::vec3(-21.91f, 0, -4.47f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // C3
        VertexFormat(glm::vec3(-21.86f, 0, -3.68f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // B3
        VertexFormat(glm::vec3(-21.73f, 0, -2.8f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // A3

        // Z2 - A2
        VertexFormat(glm::vec3(-21.16f, 0, -2.28f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Z2
        VertexFormat(glm::vec3(-20.68f, 0, -1.76f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // W2
        VertexFormat(glm::vec3(-19.98f, 0, -1.1f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // V2
        VertexFormat(glm::vec3(-19.33f, 0, -0.93f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // U2
        VertexFormat(glm::vec3(-18.55f, 0, -0.84f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // T2
        VertexFormat(glm::vec3(-17.63f, 0, -0.84f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // S2
        VertexFormat(glm::vec3(-17.02f, 0, -1.15f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // R2
        VertexFormat(glm::vec3(-16.07f, 0, -1.28f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Q2
        VertexFormat(glm::vec3(-15.11f, 0, -1.8f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // P2
        VertexFormat(glm::vec3(-14.28f, 0, -2.41f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // O2
        VertexFormat(glm::vec3(-13.54f, 0, -3.15f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // N2
        VertexFormat(glm::vec3(-12.63f, 0, -3.67f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // M2
        VertexFormat(glm::vec3(-11.67f, 0, -4.24f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // L2
        VertexFormat(glm::vec3(-10.71f, 0, -4.94f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // K2
        VertexFormat(glm::vec3(-9.66f, 0, -5.76f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // J2
        VertexFormat(glm::vec3(-8.71f, 0, -6.29f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I2
        VertexFormat(glm::vec3(-7.62f, 0, -6.55f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H2
        VertexFormat(glm::vec3(-6.27f, 0, -6.11f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // G2
        VertexFormat(glm::vec3(-5.35f, 0, -5.63f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // F2
        VertexFormat(glm::vec3(-4.31f, 0, -4.81f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // E2
        VertexFormat(glm::vec3(-3.45f, 0, -4.14f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // D2
        VertexFormat(glm::vec3(-2.95f, 0, -3.39f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // C2
        VertexFormat(glm::vec3(-2.58f, 0, -2.68f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // B2
        VertexFormat(glm::vec3(-2.45f, 0, -2.06f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // A2

        // Z1 - A1
        VertexFormat(glm::vec3(-2.54f, 0, -1.31f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Z1
        VertexFormat(glm::vec3(-2.74f, 0, -0.6f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // W1
        VertexFormat(glm::vec3(-3.08f, 0, 0.52f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // V1
        VertexFormat(glm::vec3(-3.49f, 0, 1.11f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // U1
        VertexFormat(glm::vec3(-3.91f, 0, 1.61f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // T1
        VertexFormat(glm::vec3(-4.91f, 0, 2.31f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // S1
        VertexFormat(glm::vec3(-5.7f, 0, 2.81f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // R1
        VertexFormat(glm::vec3(-6.7f, 0, 3.44f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Q1
        VertexFormat(glm::vec3(-7.57f, 0, 3.98f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // P1
        VertexFormat(glm::vec3(-8.24f, 0, 4.52f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // O1
        VertexFormat(glm::vec3(-8.99f, 0, 5.06f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // N1
        VertexFormat(glm::vec3(-9.86f, 0, 5.6f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // M1
        VertexFormat(glm::vec3(-10.86f, 0, 6.1f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // L1
        VertexFormat(glm::vec3(-11.86f, 0, 6.44f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // K1
        VertexFormat(glm::vec3(-12.7f, 0, 6.85f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // J1
        VertexFormat(glm::vec3(-13.45f, 0, 7.19f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I1
        VertexFormat(glm::vec3(-14.03f, 0, 7.44f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H1
        VertexFormat(glm::vec3(-14.9f, 0, 7.6f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // G1
        VertexFormat(glm::vec3(-15.82f, 0, 7.68f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // F1
        VertexFormat(glm::vec3(-16.44f, 0, 7.77f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // E1
        VertexFormat(glm::vec3(-17.19f, 0, 8.1f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // D1
        VertexFormat(glm::vec3(-17.9f, 0, 8.31f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // C1
        VertexFormat(glm::vec3(-18.52f, 0, 8.6f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // B1
        VertexFormat(glm::vec3(-19.11f, 0, 9.1f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // A1

        // Z - A
        VertexFormat(glm::vec3(-19.61f, 0, 9.68f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // V
        VertexFormat(glm::vec3(-20.07f, 0, 10.35f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // U
        VertexFormat(glm::vec3(-20.4f, 0, 11.18f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // T
        VertexFormat(glm::vec3(-20, 0, 12), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // S
        VertexFormat(glm::vec3(-19.23f, 0, 12.39f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // R
        VertexFormat(glm::vec3(-18.27f, 0, 12.6f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Q
        VertexFormat(glm::vec3(-17.23f, 0, 12.47f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // P
        VertexFormat(glm::vec3(-16.4f, 0, 12.1f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // O
        VertexFormat(glm::vec3(-15.61f, 0, 11.68f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // N
        VertexFormat(glm::vec3(-14.74f, 0, 11.43f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // M
        VertexFormat(glm::vec3(-13.69f, 0, 11.39f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // L
        VertexFormat(glm::vec3(-12.53f, 0, 11.39f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // K
        VertexFormat(glm::vec3(-11.4f, 0, 11.43f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // J
        VertexFormat(glm::vec3(-10.5f, 0, 11.25f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // I
        VertexFormat(glm::vec3(-9.55f, 0, 10.95f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // H
        VertexFormat(glm::vec3(-8.54f, 0, 10.65f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // G
        VertexFormat(glm::vec3(-7.49f, 0, 10.24f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // F
        VertexFormat(glm::vec3(-6.54f, 0, 9.94f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // E
        VertexFormat(glm::vec3(-5.63f, 0, 9.64f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // D
        VertexFormat(glm::vec3(-4.78f, 0, 9.34f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // C
        VertexFormat(glm::vec3(-3.93f, 0, 9.04f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // B
        VertexFormat(glm::vec3(-3.12f, 0, 8.64f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // W
        VertexFormat(glm::vec3(-2.17f, 0, 8.29f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // Z
        VertexFormat(glm::vec3(-1.25f, 0, 8.1f), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), // A
    };

    // Generare puncte de exterior si interior pentru pista
    std::vector<VertexFormat> verticesInnerAndOuter;
    glm::vec3 D, P, R, A;

    float distRed = 0.4f;
    float distBlue = 0.5f;

    glm::vec3 color = glm::vec3(0.286f, 0.125f, 0);
    glm::vec3 UP = glm::vec3(0, 1, 0);

    // Pentru interpolare
    glm::vec3 new_pos;
    glm::vec3 pos1, pos2;

    // Creez un nou vector cu vertecsi rezultati in urma interpolarii
    for (int i = 0; i < vertices.size() - 1; i++) {
        pos1 = vertices[i].position;
        pos2 = vertices[i + 1].position;

        moreVertices.push_back(vertices[i]);

        for (float j = 0.01f; j <= 0.99f; j = j + 0.01f) {
            new_pos = glm::mix(pos1, pos2, j);
            moreVertices.push_back(VertexFormat(new_pos));
        }
    }

    // Parcurg vectorul creat in urma interpolarii si compun
    // punctele de exterior si interior ale pistei dupa
    // formulele din enunt
    for (int i = 0; i < moreVertices.size() - 1; i++) {
        D = moreVertices[i + 1].position - moreVertices[i].position;
        P = glm::normalize(glm::cross(D, UP));
        R = moreVertices[i].position + distRed * P;
        A = moreVertices[i].position - distBlue * P;

        // Interior
        inner.push_back(A);
        // Exterior
        outer.push_back(R);

        // Interior
        verticesInnerAndOuter.push_back(VertexFormat(A, color));
        // Exterior
        verticesInnerAndOuter.push_back(VertexFormat(R, color));
    }

    // Creez vectori de vertecsi pentru pozitiile unde vor fi desenati copaceii
    float distTreeInt = 0.8f;
    float distTreeExt = 0.9f;

    for (int i = 0; i < vertices.size() - 1; i++) {
        D = vertices[i + 1].position - vertices[i].position;
        P = glm::normalize(glm::cross(D, UP));
        R = vertices[i].position + distRed * P;
        A = vertices[i].position - distBlue * P;

        outerTree.push_back(vertices[i].position + distTreeExt * P);
        innerTree.push_back(vertices[i].position - distTreeInt * P);
    }

    Mesh* track = new Mesh("track");

    std::vector<unsigned int> indices;
    for (int i = 0; i < verticesInnerAndOuter.size(); i++) {
        indices.push_back(i);
    }
    indices.push_back(0);
    indices.push_back(1);

    track->SetDrawMode(GL_TRIANGLE_STRIP);

    track->InitFromData(verticesInnerAndOuter, indices);

    return track;
}
