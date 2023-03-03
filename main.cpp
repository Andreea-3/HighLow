#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>
#include <cstring>
#include <stdio.h>
#include <conio.h>

#define FUNDAL (Color){112, 219, 112, 255}
#define ALBASTRU (Color) {165, 221, 245, 255}
#define ROSU (Color) {243, 168, 165, 255}


typedef enum GameScreen { REGULI = 0, AFISARE, MARE, MIC} GameScreen;


static const int screenWidth = 1200;
static const int screenHeight = 980;

struct carte
{
    int simbol;
    int valoare;
    Texture2D imgCarte;
    carte *urm;
} *carti;

carte *generarePachet()
{
    int v[52] = {0}, x, simb, val;
    int cr = 52;
    carte *p = NULL, *q;
    char sir[100], s2[5];

    while (cr > 0)
    {
        x = rand() % 52;
        if (v[x] == 0)
        {
            v[x] = 1;
            cr--;
            simb = x % 4;
            val = x % 13 + 1;
            if (val > 10)
            {
                val++;
            }
            q = new carte;
            q->simbol = simb;
            q->valoare = val;
            strcpy(sir, "./assets/c");
            itoa(val, s2, 10);
            strcat(sir, s2);
            strcat(sir, "_");
            itoa(simb, s2, 10);
            strcat(sir, s2);
            strcat(sir, ".png");
            q->imgCarte = LoadTexture(sir);
            q->urm = p;
            p = q;
        }
    }
    return p;
}

int main(void)
{
    InitAudioDevice();
    Sound PB = LoadSound("./assets/game buttons.mp3");

    InitWindow(screenWidth, screenHeight, "High Low");

    Texture2D Spate = LoadTexture("./assets/s.png");
    Texture2D F1 = LoadTexture("./assets/fundal1.png");
    Texture2D F2 = LoadTexture("./assets/fundal2.png");
    Texture2D MV= LoadTexture("./assets/mv.png");
    Texture2D MD= LoadTexture("./assets/md.png");
    Texture2D f= LoadTexture("./assets/f.png");
    GameScreen currentScreen = REGULI;


    int framesCounter = 0;         
    bool nou= false;

    Texture2D m = LoadTexture("./assets/anm.png");
    float frameWidth= (float)(m.width/16);
    int maframe=(int)(m.width/ (int) frameWidth);
    float t = 0.0f;
    int frame = 0;


    Texture2D Professor= LoadTexture("./assets/PU.png");
    Texture2D X= LoadTexture("./assets/X.png");

    SetTargetFPS(60);  

    carti= generarePachet();
    carte *q;
    carte *p; 
    carte *a;

    while (!WindowShouldClose())   
    {
        if(nou){
            carti=generarePachet();
            nou=false;
        }
        switch(currentScreen)
        {
            case REGULI:
            {
                framesCounter++;    

                if (framesCounter > 360)
                {
                    currentScreen = AFISARE;
                    PlaySound(PB);
                }
            } break;
            case AFISARE:
            {
                if (IsKeyPressed(KEY_H))
                {
                    PlaySound(PB);
                    currentScreen = MARE;
                }
                else{
                    if (IsKeyPressed(KEY_L))
                    {
                        PlaySound(PB);
                        currentScreen = MIC;
                    }
                }
            } break;
            case MARE:
            {
                if (IsKeyPressed(KEY_R))
                {
                    nou=true;
                    PlaySound(PB);
                    currentScreen = AFISARE;
                }
            } break;
            case MIC:
            {
                if (IsKeyPressed(KEY_R))
                {
                    nou=true;
                    PlaySound(PB);
                    currentScreen = AFISARE;
                }
            } break;
        }
        p = carti;
        q = p; 
        p = p -> urm;
        a= p;
        BeginDrawing();

            ClearBackground(FUNDAL);

            switch(currentScreen)
            {
                case REGULI:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, LIGHTGRAY);
                    DrawText("Rivalul cel mare, Mojo Jojo vrea numaidecat sa distruga orasul!", 70, 50, 35, RED);
                    DrawText("Ajuta-le pe Fetitele PowerPuff sa castige printr-un joc de High-Low.", 100, 100, 20, BLACK);
                    DrawText("Pentru a-l învinge trebuie ghicesti daca cartea e mai mare sau mai mica.", 100, 130, 20, BLACK);
                    DrawText("In acest mod vom putea decide cine va invinge lupta!", 100, 160, 20, BLACK);
                    DrawTexture(f, 250, 200, RAYWHITE);

                } break;
                case AFISARE:
                {

                    t+=GetFrameTime();// cat timp trb pt un frame
                    if(t>=0.4){
                        t=0.0f;
                        frame += 1;
                    }
                    frame=frame % maframe;
                    DrawTextureRec(
                        m,
                        Rectangle{
                            (frameWidth*frame),
                            0,
                            frameWidth, 
                            (float) m.height},
                        Vector2{520, 0},
                        RAYWHITE
                    );
                    DrawTexture(q -> imgCarte,300, 280, RAYWHITE);
                    DrawTexture(Spate, 648, 280, RAYWHITE);

                    DrawTexture(Professor, 700, 500, RAYWHITE);
                    DrawTexture(X,  60, 70, RAYWHITE);

                    DrawText("Urmatoarea carte este mai mica sau mai mare?", 110, 180, 40, DARKGREEN);
                    DrawText("Apasa [H] daca crezi ca urmeaza o carte mai mare sau [L] daca consideri ca e mai mica.", 120, 220, 20, DARKGREEN);

                    if (IsKeyPressed(KEY_ENTER))
                    {
                        UnloadTexture(q-> imgCarte);
                    }


                } break;
                case MARE:
                {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                    if(q -> valoare <= a-> valoare){
                        DrawTexture(F1, 0, 0, RAYWHITE);
                        DrawRectangle(190, 15, 900, 91, ROSU);
                        DrawText("Ai ghicit bine aceasta carte.", 200, 20, 40, ALBASTRU);
                        DrawText("Apasa R pentru a continua (esti blocat intr-un timeloop).", 195, 60, 30, ALBASTRU);
                        DrawTexture(MD, 580, 470, RAYWHITE);
                        DrawTexture(a -> imgCarte, 300, 280, RAYWHITE);
                    }
                    else{
                        DrawTexture(F2, 0, 0, RAYWHITE);
                        DrawRectangle(185, 15, 900, 115, ALBASTRU);
                        DrawText("Ai raspuns gresit.", 200, 20, 40, ROSU);
                        DrawText(" Mojo Jojo vrea sa distruga orasul.", 200, 60, 40, ROSU);
                        DrawText("Apasa R pentru a incerca iar.", 200, 100, 30, ROSU);
                        DrawTexture(MV, 580, 470, RAYWHITE);
                        DrawTexture(a -> imgCarte, 300, 280, RAYWHITE);
                    }
                } break;
                case MIC:
                {
                    DrawTexture(a->imgCarte, 300, 280, RAYWHITE);
                    if(q -> valoare >= a-> valoare){
                        DrawTexture(F1, 0, 0, RAYWHITE);
                        DrawRectangle(190, 15, 900, 91, ROSU);
                        DrawText("Ai ghicit bine aceasta carte.", 200, 20, 40, ALBASTRU);
                        DrawText("Apasa R pentru a continua (esti blocat intr-un timeloop).", 195, 60, 30, ALBASTRU);
                        DrawTexture(MD, 580, 470, RAYWHITE);
                        DrawTexture(a -> imgCarte, 300, 280, RAYWHITE);

                    }
                    else{
                        DrawTexture(F2, 0, 0, RAYWHITE);
                        DrawRectangle(185, 15, 900, 115, ALBASTRU);
                        DrawText("Ai raspuns gresit.", 200, 20, 40, ROSU);
                        DrawText(" Mojo Jojo vrea sa distruga orasul.", 200, 60, 40, ROSU);
                        DrawText("Apasa R pentru a incerca iar.", 200, 100, 30, ROSU);
                        DrawTexture(MV, 580, 470, RAYWHITE);
                        DrawTexture(a -> imgCarte, 300, 280, RAYWHITE);
                    }
                } break;
            }
        EndDrawing();
        
    }

    CloseWindow();        
    return 0;
}
