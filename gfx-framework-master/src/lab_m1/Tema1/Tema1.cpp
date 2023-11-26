#include <vector>
#include <iostream>

#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"

#include "math.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Coltul stanga-jos
    glm::vec3 corner = glm::vec3(0, 0, 0);

    // Initializarea pasilor de translatie
    translateX = 0;
    translateY = 0;

    // Initializarea pasilor de scalare
    scaleX = 1;
    // Latimea dreptunghiului de scor este 60 dupa calcule, deci
    // voi scala pe axa verticala cu jumatate din 60 pentru a nu
    // depasi conturul
    scaleY = widthScoreRectangle / 2;

    // Initializarea dimensiunilor partilor corpului
    dimHead = 30;
    dimBody = 32;
    dimBeak = 6;
    dimWing = 18;

    // Initializarea unghiului de inclinare al ratei
    angle = RADIANS(30);

    angularStep1 = 0;
    angularStep2 = 0;

    // Initial rata se deplaseaza spre dreapta, in sus
    goRightUp = true;
    goRightDown = false, goLeftUp = false, goLeftDown = false;

    currTime = 0;
    // Initial avem doar o rata
    numDucks = 1;

    // Stare care indica daca o rata a fost impuscata sau nu
    duckShot = false;
    // Stare care indica daca o rata a evadat sau nu
    duckEscape = false;

    numBullets = 3, numLives = 3;
    // Scorul din logica jocului. Creste pana la 100 din 5 in 5
    score = 0;
    // Scorul pentru desenarea chenarului de scor. Creste pana la 240 din 12 in 12
    drawnScore = 0;

    // Crearea partilor componente ale pasarii
    Mesh* head = object2D::CreateCircle("head", glm::vec3(txHead, tyHead, 0), dimHead, glm::vec3(0, 0.192f, 0), true);
    AddMeshToList(head);

    Mesh* beak = object2D::CreateTriangle("beak", glm::vec3(txBeak, tyBeak, 0), dimBeak, glm::vec3(0.854f, 0.647f, 0.125f), true);
    AddMeshToList(beak);

    Mesh* body = object2D::CreateTriangle("body", glm::vec3(txBody, tyBody, 0), dimBody, glm::vec3(0.36f, 0.25f, 0.2f), true);
    AddMeshToList(body);

    // aripa de jos
    Mesh* wingBottom = object2D::CreateTriangle("wingBottom", glm::vec3(txWingBottom, tyWingBottom, 0), dimWing, glm::vec3(0.36f, 0.25f, 0.2f), true);
    AddMeshToList(wingBottom);

    // aripa de sus
    Mesh* wingTop = object2D::CreateTriangle("wingTop", glm::vec3(txWingTop, tyWingTop, 0), dimWing, glm::vec3(0.36f, 0.25f, 0.2f), true);
    AddMeshToList(wingTop);

    // Decor

    // Scorul
    Mesh* totalScore = object2D::CreateSquare("totalScore", glm::vec3(txScore, tyScore, 0), widthScoreRectangle / 2, 8, 2, glm::vec3(0, 0, 1), false);
    AddMeshToList(totalScore);

    Mesh* currScore = object2D::CreateSquare("currScore", glm::vec3(txScore, tyScore, 0), 1, 8, 2, glm::vec3(0, 0, 1), true);
    AddMeshToList(currScore);

    // Vietile
    Mesh* life1 = object2D::CreateCircle("life1", glm::vec3(1160, 683, 0), 17, glm::vec3(1, 0, 0), true);
    AddMeshToList(life1);

    Mesh* life2 = object2D::CreateCircle("life2", glm::vec3(1200, 683, 0), 17, glm::vec3(1, 0, 0), true);
    AddMeshToList(life2);

    Mesh* life3 = object2D::CreateCircle("life3", glm::vec3(1240, 683, 0), 17, glm::vec3(1, 0, 0), true);
    AddMeshToList(life3);

    // Gloantele
    Mesh* bullet1 = object2D::CreateSquare("bullet1", glm::vec3(1150, 629, 0), 20, 1, 1.5f, glm::vec3(0, 0.39f, 0), false);
    AddMeshToList(bullet1);

    Mesh* bullet2 = object2D::CreateSquare("bullet2", glm::vec3(1190, 629, 0), 20, 1, 1.5f, glm::vec3(0, 0.39f, 0), false);
    AddMeshToList(bullet2);

    Mesh* bullet3 = object2D::CreateSquare("bullet3", glm::vec3(1230, 629, 0), 20, 1, 1.5f, glm::vec3(0, 0.39f, 0), false);
    AddMeshToList(bullet3);

    // Iarba
    Mesh* grass = object2D::CreateSquare("grass", glm::vec3(0, 0, 0), 160, 8, 1, glm::vec3(0.196f, 0.803f, 0.196f), true);
    AddMeshToList(grass);
}



void Tema1::FrameStart()
{
    glClearColor(0.529f, 0.807f, 0.980f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Desenare scor maxim
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["totalScore"], shaders["VertexColor"], modelMatrix);

    // Numar de pasi necesari
    if (scaleX < (lengthScoreRectangle / 2)) {
        scaleX = drawnScore / 8;
    }

    // Desenare scor variabil
    modelMatrix *= transform2D::Translate(txScore, tyScore);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);
    modelMatrix *= transform2D::Translate(-txScore, -tyScore);
    RenderMesh2D(meshes["currScore"], shaders["VertexColor"], modelMatrix);
    
    // Desenare iarba
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

    // Desenare rata
    // Rata este pozitionata initial sub un unghi de 30 de grade.
    // Rotirea se face in jurul centrului de greutate al ratei,
    // calculat la coordonatele (82, 62).
    modelMatrix *= transform2D::Translate(translateX, translateY);

    modelMatrix *= transform2D::Translate(txWeightCenter, tyWeightCenter);
    modelMatrix *= transform2D::Rotate(angle);
    modelMatrix *= transform2D::Translate(-txWeightCenter, -tyWeightCenter);

    // Randare parti componente rata
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

    currTime += deltaTimeSeconds;

    // ------------------------ LOGICA JOCULUI ------------------------

    // Resetarea numarului de vieti si de gloante in cazul in care
    // jucatorul nu mai are vieti
    if (numLives == 0) {
        numLives = 3;
        numBullets = 3;
    }

    // Resetarea scorului, numarului de vieti si de gloante in cazul
    // in care jucatorul a ajuns la scorul maxim posibil
    if (score == 100 && drawnScore == 240) {
        cout << "Score is " << score << ". We have a WINNER!:)" << endl;
        score = 0;
        drawnScore = 0;
        numLives = 3;
        numBullets = 3;
    }

    // Rata scapa
    if (duckEscape == true) {
        // Actualizez numarul de gloante ramase in cazul
        // in care s-a dat click
        if (click == true) {
            numBullets--;
        }

        // Cum rata a scapat, jucatorul pierde o viata
        numLives--;

        // Resetarea numarului de vieti
        if (numLives == 0) {
            cout << "You lost all your lives. GAME OVER! :(" << endl;
            numLives = 3;
        }

        // Resetarea numarului de gloante
        if (numBullets == 0) {
            numBullets = 3;
        }

        click = false;

        cout << "Remained bullets: " << numBullets << endl;
        cout << "Remained lives: " << numLives << endl;
        cout << endl;
    }

    // Se joaca in continuare pana la atingerea scopului
    if ((score < 100 && drawnScore < 240) || numLives > 0) {
        // Evadare rata.
        // Rata evadeaza in cazul in care trec 5 secunde si nu a fost nimerita.
        // De asemenea, o rata care deja evadeaza nu mai poate fi impuscata
        if ((currTime > 5 && duckShot == false) || (duckShot == true && ok == 1)) {
            // Rata isi schimba directia in sus, rotindu-se si translatandu-se corespunzator
            translateY += deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;
            angle = RADIANS(90);
            
            ok = 1;

            // Verificare depasire limite ecran
            if (translateY >= resolution.y) {
                ok = 0;

                // Resetare coordonate
                translateX = 0;
                translateY = 0;

                // Resetare unghi de inclinare al ratei
                angle = RADIANS(30);

                // Resetarea pozitiei din care pleaca rata
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(translateX, translateY);

                // Resetarea pozitiei initiale a ratei
                modelMatrix *= transform2D::Translate(txWeightCenter, tyWeightCenter);
                modelMatrix *= transform2D::Rotate(angle);
                modelMatrix *= transform2D::Translate(-txWeightCenter, -tyWeightCenter);

                // Resetarea timpului
                currTime = 0;

                // Resetarea directiilor
                goRightUp = true;
                goRightDown = false;
                goLeftUp = false;
                goLeftDown = false;

                // Resetare numar vieti
                if (numLives == 0) {
                    numLives = 3;
                }

                // Resetare numar gloante
                if (numBullets == 0) {
                    numBullets = 3;
                }

                // Resetare pentru urmatoarele rate
                duckEscape = true;
                duckShot = false;

                // Aparitie urmatoarea rata (pentru incrementarea vitezei)
                numDucks++;
            }
        } else if (duckShot == true && ok == 0) {
            // Impuscare rata.
            // Marchez starea ca rata nu mai poate scapa, din moment ce a fost nimerita
            duckEscape = false;

            // Rata isi schimba directia in jos, rotindu-se si translatandu-se corespunzator
            translateY -= deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;
            angle = RADIANS(270);

            // Verificare depasire limite ecran
            if (translateY < 0) {
                cout << "Remained bullets: " << numBullets << endl;
                cout << "Remained lives: " << numLives << endl;
                cout << endl;

                // Resetare coordonate
                translateX = 0;
                translateY = 0;

                // Resetare unghi de inclinare al ratei
                angle = RADIANS(30);

                // Resetarea pozitiei din care pleaca rata
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(translateX, translateY);

                // Resetarea pozitiei initiale a ratei
                modelMatrix *= transform2D::Translate(txWeightCenter, tyWeightCenter);
                modelMatrix *= transform2D::Rotate(angle);
                modelMatrix *= transform2D::Translate(-txWeightCenter, -tyWeightCenter);

                // Resetarea timpului
                currTime = 0;

                // Resetarea directiilor
                goRightUp = true;
                goRightDown = false;
                goLeftUp = false;
                goLeftDown = false;

                // Resetare pentru urmatoarele rate (pot fi nimerite)
                duckShot = false;
                duckEscape = false;

                // Resetare numar vieti
                if (numLives == 0) {
                    numLives = 3;
                }

                // Resetare numar gloante
                if (numBullets == 0) {
                    numBullets = 3;
                }

                // Aparitie urmatoarea rata
                numDucks++;
            }
        } else {
            MovementsDuck(deltaTimeSeconds);
            duckEscape = false;
        }
    } 



    // ------------------------------ ARIPA DE JOS -----------------------------------

    // Aripa de jos se va roti cu 360 - 90 = 270 de grade fata de coltul de sus al
    // triunghiului, aflat la coordonatele (90, 48), pentru a veni in jos.
    modelMatrix *= transform2D::Translate(90, 48) * transform2D::Rotate(RADIANS(270))
        * transform2D::Translate(-90, -48);

    // Animatia de miscare a aripii, cu un unghi care creste progresiv de la 0
    // pana la o valoare 0.3
    if (angularStep1 <= 0.3) {
        angularStep1 += deltaTimeSeconds;
    }
    else {
        angularStep1 = 0;
    }

    modelMatrix *= transform2D::Translate(90, 48) * transform2D::Rotate(angularStep1)
        * transform2D::Translate(-90, -48);

    // Randare pe ecran aripa
    RenderMesh2D(meshes["wingBottom"], shaders["VertexColor"], modelMatrix);


    // ------------------------------ ARIPA DE SUS ------------------------------------

    // modelMatrix trebuie redefinit; nu putem lua modelMatrix modificat deja de
    // aripa de jos.
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);

    modelMatrix *= transform2D::Translate(82, 62);
    modelMatrix *= transform2D::Rotate(angle);
    modelMatrix *= transform2D::Translate(-82, -62);

    // Aripa de sus se va roti cu 90 de grade fata de coltul de jos al triunghiului,
    // aflat la coordonatele (92, 76), pentru a veni in sus.
    modelMatrix *= transform2D::Translate(92, 76) * transform2D::Rotate(RADIANS(90))
        * transform2D::Translate(-92, -76);

    // Animatia de miscare a aripii, cu un unghi care scade progresiv de la
    //  o valoare 0 pana la -0.3
    if (angularStep2 >= -0.3) {
        angularStep2 -= deltaTimeSeconds;
    }
    else {
        angularStep2 = 0;
    }

    modelMatrix *= transform2D::Translate(92, 76) * transform2D::Rotate(angularStep2)
        * transform2D::Translate(-92, -76);

    // Randare pe ecran aripa
    RenderMesh2D(meshes["wingTop"], shaders["VertexColor"], modelMatrix);

    // Decorul desenat

    // Gloantele
    if (numBullets == 3) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["bullet3"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);
    } else if (numBullets == 2) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);
    } else if (numBullets == 1) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);
    }
    
    // Vietile
    if (numLives == 3) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["life3"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
    } else if (numLives == 2) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
    } else if (numLives == 1) {
        modelMatrix = glm::mat3(1);
        RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
    }
}

// --------------------------------- DEPLASARE RATA --------------------------------------

// Functie in care am definit toate cele 4 directii posibile in care rata se poate deplasa:
// - dreapta sus (RU)
// - dreapta jos (RD)
// - stanga sus (LU)
// - stanga jos (LD)
void Tema1::MovementsDuck(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();

    if (goRightUp) {
        translateX += deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;
        translateY += deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;

        // RU -> LU
        if (translateX >= resolution.x - 100) {
            angle = angle + (M_PI - 2 * angle);
            goRightUp = false;
            goLeftUp = true;
        }

        // RU -> RD
        if (translateY >= resolution.y - 100) {
            angle = angle - 2 * angle;
            goRightUp = false;
            goRightDown = true;
        }
    }

    if (goLeftUp) {
        translateX -= deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;
        translateY += deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;

        // LU -> LD
        if (translateY >= resolution.y - 100) {
            angle = angle - 2 * angle;
            goLeftUp = false;
            goLeftDown = true;
        }

        // LU -> RU
        if (translateX <= -50) {
            angle = angle + (M_PI - 2 * angle);
            goLeftUp = false;
            goRightUp = true;
        }
    }

    if (goLeftDown) {
        translateX -= deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;
        translateY -= deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;

        // LD -> RD
        if (translateX <= -50) {
            angle = angle + (M_PI - 2 * angle);
            goLeftDown = false;
            goRightDown = true;
        }

        // LD -> LU
        if (translateY <= -50) {
            angle = angle - 2 * angle;
            goLeftDown = false;
            goLeftUp = true;
        }
    }

    if (goRightDown) {
        translateX += deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;
        translateY -= deltaTimeSeconds * 140 + (deltaTimeSeconds * 140 * numDucks) / 5;

        // RD -> RU
        if (translateY <= -50) {
            angle = angle - 2 * angle;
            goRightDown = false;
            goRightUp = true;
        }

        // RD -> LD
        if (translateX >= resolution.x - 100) {
            angle = angle + (M_PI - 2 * angle);
            goRightDown = false;
            goLeftDown = true;
        }
    }
}


void Tema1::FrameEnd()
{
}



/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    glm::ivec2 resolution = window->GetResolution();

    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        // Activare variabila cand se da click pe ecran
        click = true;
        // Daca rata nu scapa, poate fi impuscata
        if (duckEscape == false) {
            // Coltul din stanga-jos al ecranului incadrator
            glm::vec3 cornerHitBoxLeftDown = glm::vec3(-20, -60, 1);

            // Coltul din dreapta-sus al ecranului incadrator
            glm::vec3 cornerHitBoxRightUp = glm::vec3(220, 180, 1);

            // Translatarea coordonatelor de interes dupa rata
            modelMatrix *= transform2D::Translate(translateX, translateY);
            cornerHitBoxRightUp = modelMatrix * cornerHitBoxRightUp;
            cornerHitBoxLeftDown = modelMatrix * cornerHitBoxLeftDown;

            // Incadrare in dreptunghiul ce inconjoara rata
            if (mouseX >= cornerHitBoxLeftDown.x && mouseX <= cornerHitBoxRightUp.x &&
                (resolution.y - mouseY) >= cornerHitBoxLeftDown.y &&
                (resolution.y - mouseY) <= cornerHitBoxRightUp.y) {
                // Marchez starea ca rata este impuscata
                duckShot = true;
                // Actualizare numar de gloante si scor
                numBullets--;
                score += 5;
                drawnScore += 12;
            } else {
                // Actualizare numar de gloante si vieti ramase
                numBullets--;
                if (numBullets == 0) {
                    numLives--;

                    if (numLives == 0) {
                        numLives = 3;
                    }
                    if (numBullets == 0) {
                        numBullets = 3;
                    }
                }
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
