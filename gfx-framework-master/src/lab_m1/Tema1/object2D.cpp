#include "lab_m1/Tema1/object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    float increaseOX,
    float increaseOY,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;  

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(increaseOX * length, 0, 0), color),
        VertexFormat(corner + glm::vec3(increaseOX * length, increaseOY * length, 0), color),
        VertexFormat(corner + glm::vec3(0, increaseOY * length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(4 * length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, 2 * length, 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 point,
    float dimension,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices; 
    std::vector<unsigned int> indices;

    vertices.emplace_back(point, color);

    float arg;

    Mesh* circle = new Mesh(name);

    for (int i = 0; i < NUM_TRIANGLES; ++i) {
        arg = i * TWO_PI / 100;

        vertices.emplace_back(glm::vec3(cos(arg) * dimension + point.x, sin(arg) * dimension + point.y, 0), color);
        indices.push_back(i);
    }
    
    if (fill) {
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }
    else {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
   
    
    circle->InitFromData(vertices, indices);
    return circle;
}
