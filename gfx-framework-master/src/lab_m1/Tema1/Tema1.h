#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {

     public:
        Tema1();
        ~Tema1();

        void Init() override;
        

     private:
        void MovementsDuck(float deltaTimeSeconds);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        glm::mat3 modelMatrix;

        float translateX, translateY;
        float angularStep1, angularStep2;
        float scaleX, scaleY;
        
        float txHead = 156, tyHead = 64;
        float txBeak = 180, tyBeak = 50;
        float txBody = 36, tyBody = 30;
        float txWingBottom = 90, tyWingBottom = 22;
        float txWingTop = 90, tyWingTop = 68;
        float txScore = 1020, tyScore = 560;
        float txWeightCenter = 82, tyWeightCenter = 62;

        float dimHead, dimBody, dimBeak, dimWing;

        bool goRightUp, goRightDown, goLeftUp, goLeftDown;

        float angle;

        float currTime;

        int numDucks;

        bool duckShot, duckEscape;
        int numBullets, numLives;

        int score;
        float drawnScore;
        float lengthScoreRectangle = 240;
        float widthScoreRectangle = 60;

        // Variabila care indica daca am dat click sau nu pe ecran
        bool click = false;
        // Variabila suplimentara pentru a verifica daca rata a evadat
        int ok = 0;
    };
}   // namespace m1
