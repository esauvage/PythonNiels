// /*
//  * Luke Skycodeur (niels@les-sauvages.fr)
//  * 1er Novembre 2025
//  *
//  * Le jeu de la vie avec Raylib
//  *
//  * Programme qui simule le jeu de la vie, en utilisant la librairie graphique Raylib
//  */

// #include <raylib.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <pthread.h>

// #define NBPIXELSX 1080
// #define NBPIXELSY 720

// const int nbCasesX = NBPIXELSX / 10;
// const int nbCasesY = NBPIXELSY / 10;

// // A supprimer. (Remplacer par un tableau d'int)
// typedef struct
// {
//     int couleur;
// } Point;

// /*
//  * Dessine la grille sur l'écran
//  */
// void dessinerGrille()
// {
//     for (int i = 10; i < NBPIXELSX; i += 10)
//     {
//         DrawLine(i, 0, i, NBPIXELSY, BLACK);
//     }
//     for (int i = 10; i < NBPIXELSY; i += 10)
//     {
//         DrawLine(0, i, NBPIXELSX, i, BLACK);
//     }
// }

// /*
//  * Dessine les cellules vivantes sur l'écran
//  */
// void dessinerPointsNoirs(Point ** points)
// {
//     for (int y = 0; y < nbCasesY; ++y)
//     {
//         for (int x = 0; x < nbCasesX; ++x)
//         {
//             if (points[y][x].couleur == 1) DrawRectangle(x * 10, y * 10, 10, 10, BLACK);
//         }
//     }
// }

// // Copie un tableau de points dans un autre tableau de points
// void copier(Point ** points, Point ** pointsTemp)
// {
//     for (int y = 0; y < nbCasesY; ++y)
//     {
//         for (int x = 0; x < nbCasesX; ++x)
//         {
//             pointsTemp[y][x] = points[y][x];
//         }
//     }
// }

// /*
//  * Initialise toutes les cellules à l'état mort
//  */
// void initPoints(Point ** points)
// {
//     for (int y = 0; y < nbCasesY; y++)
//     {
//         for (int x = 0; x < nbCasesX; x++)
//         {
//             points[y][x].couleur = 0;
//         }
//     }
// }

// void rajouterCellules(Point ** points)
// {
//     Vector2 positionSouris;
//     if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
//     {
//         positionSouris = GetMousePosition();

//         char nbX[108];
//         char nbY[108];

//         sprintf(nbX, "%d", (int)positionSouris.x);
//         sprintf(nbY, "%d", (int)positionSouris.y);

//         int x, y;

//         // Supprimer le chiffre des unités
//         nbX[strlen(nbX) - 1] = '\0';
//         nbY[strlen(nbY) - 1] = '\0';

//         x = atoi(nbX);
//         y = atoi(nbY);

//         if (points[y][x].couleur == 1) points[y][x].couleur = 0;
//         else if (points[y][x].couleur == 0) points[y][x].couleur = 1;
//     }
// }

// void sauvegarder(Point ** points)
// {
//     FILE * fichier = fopen("clown.txt", "w");
//     if (!fichier)
//     {
//         perror("Erreur lors de l'ouverture du fichier 'clown.txt' en écriture");
//         return;
//     }

//     for (int y = 0; y < nbCasesY; ++y)
//     {
//         for (int x = 0; x < nbCasesX; ++x)
//         {
//             if (points[y][x].couleur == 1)
//             {
//                 fprintf(fichier, "%d %d\n", x, y);
//             }
//         }
//     }

//     fclose(fichier);
// }

// int main()
// {
//     InitWindow(NBPIXELSX, NBPIXELSY, "Game of life - Mode édition");
//     SetTargetFPS(60);

//     Image boutonEnregistrer = LoadImage("BoutonEnregistrer.png");
//     Texture2D textureBoutonEnregistrer = LoadTextureFromImage(boutonEnregistrer);

//     UnloadImage(boutonEnregistrer);

//     Point ** points;

//     points = malloc(nbCasesY * sizeof(Point *));
//     for (int i = 0; i < nbCasesY; ++i)
//     {
//         points[i] = malloc(nbCasesX * sizeof(Point));
//     }

//     initPoints(points);

//     Rectangle bouton = {
//         1000,
//         680,
//         70,
//         30,
//     };

//     while (!WindowShouldClose())
//     {
//         Vector2 souris = GetMousePosition();

//         if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(souris, bouton)) sauvegarder(points);

//         BeginDrawing();

//         ClearBackground(RAYWHITE);

//         dessinerGrille();
//         dessinerPointsNoirs(points);
//         DrawTexture(textureBoutonEnregistrer, 1000, 680, WHITE);

//         rajouterCellules(points);

//         EndDrawing();
//     }
//     CloseWindow();
//     UnloadTexture(textureBoutonEnregistrer);

//     for (int i = 0; i < nbCasesY; ++i)
//     {
//         free(points[i]);
//     }
//     free(points);

//     return 0;
// }
