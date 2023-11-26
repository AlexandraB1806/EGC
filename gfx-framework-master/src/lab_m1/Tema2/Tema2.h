#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };


        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void Tema2::RenderScene();
        bool Tema2::IsInsideTrack(Mesh* track);
        void Tema2::GenerateEnemyTrajectory(std::vector<VertexFormat> points);


    protected:
        implemented::CameraTema2* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        ViewportArea miniViewportArea;

        std::vector<glm::vec3> innerPoints;
        std::vector<glm::vec3> outerPoints;
        std::vector<VertexFormat> moreVertices;
        
        float tx, tz;
        float angle;
        glm::vec3 carCenter;
        std::vector<VertexFormat> verticesInnerAndOuter;

        // Iarba
        std::vector<Mesh*> grass;

        // Copaci
        std::vector<glm::vec3> outerTree;
        std::vector<glm::vec3> innerTree;

        // Adversari
        int id1, id2, id3, id4, id5, id6, id7, id8, id9, id10;
        std::vector<VertexFormat> verticesEnemy1;
        std::vector<VertexFormat> verticesEnemy2;
        std::vector<VertexFormat> verticesEnemy3;
        std::vector<VertexFormat> verticesEnemy4;
        std::vector<VertexFormat> verticesEnemy5;
        std::vector<VertexFormat> verticesEnemy6;
        std::vector<VertexFormat> verticesEnemy7;
        std::vector<VertexFormat> verticesEnemy8;
        std::vector<VertexFormat> verticesEnemy9;
        std::vector<VertexFormat> verticesEnemy10;
    };
}   // namespace m1
