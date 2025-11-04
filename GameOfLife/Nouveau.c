#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define NBPIXELSX 1080
#define NBPIXELSY 720

const int nbCasesX = NBPIXELSX / 10;
const int nbCasesY = NBPIXELSY / 10;

// A supprimer. (Remplacer par un tableau d'int)
typedef struct
{
    int couleur;
} Point;

int lancerThread = 1;
int delai = 100;
int generation = 0;
int stop = 0;

/*
 * Lit un fichier de configuration et place les points lus dans le tableau de points.
*/
int lireConfig(Point ** points)
{
    int indexNb = 0;
    char xOuY = 'x';

    FILE * fichier = fopen("clown.txt", "r");
    if (!fichier) {return 1; puts("Erreur (l. 22)");}

    char data[nbCasesX * nbCasesY];

    ssize_t nbLues = fread(data, sizeof(char), nbCasesX * nbCasesY, fichier);
    data[nbLues] = '\0';

    // Contient temporairement la chaine de caractères qui représente le nombre en train d'être lu. (Pour les absices)
    char nbX[5];
    nbX[0] = '\0';

    // Contient temporairement la chaine de caractères qui représente le nombre en train d'être lu. (Pour les ordonnées)
    char nbY[5];
    nbY[0] = '\0';
    for (int i = 0; i < (int)strlen(data); ++i)
    {
        // les caractères de la table ASCII représentant des chiffres
        if (data[i] > 47 && data[i] < 58)
        {
            if (xOuY == 'x')
            {
                nbX[indexNb] = data[i];
                nbX[indexNb + 1] = '\0';

                indexNb++;
            }
            else if (xOuY == 'y')
            {
                nbY[indexNb] = data[i];
                nbY[indexNb + 1] = '\0';

                indexNb++;
            }
        }
        else
        {
            indexNb = 0;

            // Ajout dans le tableau de points
            if (xOuY == 'y') {xOuY = 'x'; points[atoi(nbY)][atoi(nbX)].couleur = 1;}
            else if (xOuY == 'x') xOuY = 'y';
        }
    }

    fclose(fichier);
    return 1;
}

/*
 * Dessine la grille sur l'écran
 */
void dessinerGrille()
{
    for (int i = 10; i < NBPIXELSX; i += 10)
	{
        DrawLine(i, 0, i, NBPIXELSY, BLACK);
	}
    for (int i = 10; i < NBPIXELSY; i += 10)
	{
        DrawLine(0, i, NBPIXELSX, i, BLACK);
	}
}

/*
 * Dessine les cellules vivantes sur l'écran
 */
void dessinerPointsNoirs(Point ** points)
{
    for (int y = 0; y < nbCasesY; ++y)
	{
        for (int x = 0; x < nbCasesX; ++x)
		{
            if (points[y][x].couleur == 1) DrawRectangle(x * 10, y * 10, 10, 10, BLACK);
		}
	}
}

// Copie un tableau de points dans un autre tableau de points
void copier(Point ** points, Point ** pointsTemp)
{
    for (int y = 0; y < nbCasesY; ++y)
    {
        for (int x = 0; x < nbCasesX; ++x)
        {
            pointsTemp[y][x] = points[y][x];
        }
    }
}

void * jouerTour(void * arg)
{
    Point ** points = (Point **)arg;
    Point ** pointsTemp;

    pointsTemp = malloc(nbCasesY * sizeof(Point *));
    if (!pointsTemp) puts("Erreur malloc()");

    for (int i = 0; i < nbCasesY; ++i)
    {
        pointsTemp[i] = malloc(nbCasesX * sizeof(Point));
        if (!pointsTemp) puts("Erreur malloc()");
    }

    if (generation == 0)
    {
        usleep(delai * 1000);
    }

    copier(points, pointsTemp);

    for (int y = 0; y < nbCasesY; y++)
    {
        for (int x = 0; x < nbCasesX; x++)
        {
            int nbCasesAutour = 0;

            // La case au dessus
            if (y > 0 && points[y - 1][x].couleur == 1) nbCasesAutour++;
            // La case au dessous
            if (y < (nbCasesY - 1) && points[y + 1][x].couleur == 1) nbCasesAutour++;
            // La case à gauche
            if (x > 0 && points[y][x - 1].couleur == 1) nbCasesAutour++;
            // La case au droite
            if (x < (nbCasesX - 1) && points[y][x + 1].couleur == 1) nbCasesAutour++;
            // La case en haut à gauche
            if (y > 0 && x > 0 && points[y - 1][x - 1].couleur == 1) nbCasesAutour++;
            // La case en haut à droite
            if (y > 0 && x < (nbCasesX - 1) && points[y - 1][x + 1].couleur == 1) nbCasesAutour++;
            // La case en bas à gauche
            if (y < (nbCasesY - 1) && x > 0 && points[y + 1][x - 1].couleur == 1) nbCasesAutour++;
            // La case en bas à droite
            if (y < (nbCasesY - 1) && x < (nbCasesX - 1) && points[y + 1][x + 1].couleur == 1) nbCasesAutour++;

            if (nbCasesAutour == 3 && points[y][x].couleur == 0) pointsTemp[y][x].couleur = 1;
            if ((nbCasesAutour != 2 && nbCasesAutour != 3) && points[y][x].couleur == 1) pointsTemp[y][x].couleur = 0;
        }
    }

    copier(pointsTemp, points);

    for (int i = 0; i < nbCasesY; ++i)
    {
        free(pointsTemp[i]);
    }
    free(pointsTemp);

    // sleep(1);
    usleep(delai * 1000);

    lancerThread = 1;
    generation++;

    return NULL;
}

/*
 * Initialise toutes les cellules à l'état mort
 */
void initPoints(Point ** points)
{
    for (int y = 0; y < nbCasesY; y++)
    {
        for (int x = 0; x < nbCasesX; x++)
        {
            points[y][x].couleur = 0;
        }
    }
}

int main()
{
    InitWindow(NBPIXELSX, NBPIXELSY, "Game of life");
    SetTargetFPS(60);

    Point ** points;

    points = malloc(nbCasesY * sizeof(Point *));
    for (int i = 0; i < nbCasesY; ++i)
    {
        points[i] = malloc(nbCasesX * sizeof(Point));
    }

    initPoints(points);

    int o = lireConfig(points);
    if (!o) {return 1; puts("Erreur");}

    pthread_t threadJouerTour;

    while (!WindowShouldClose())
    {
        // Pour stopper les générations
        if (IsKeyPressed(KEY_SPACE) && stop == 0) stop = 1;
        else if (IsKeyPressed(KEY_SPACE) && stop == 1) stop = 0;

        // Pour augmenter ou diminuer la vitesse du jeu
        if (IsKeyPressed(KEY_LEFT) && delai > 0&& delai <= 1000) delai += 10;
        if (IsKeyPressed(KEY_RIGHT) && delai > 0 && delai <= 1000) delai -= 10;

        if (lancerThread && stop == 0)
        {
            pthread_join(threadJouerTour, NULL);
            pthread_create(&threadJouerTour, NULL, jouerTour, points);
            lancerThread = 0;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        dessinerGrille();
        dessinerPointsNoirs(points);

        DrawText(TextFormat("Génération : %d", generation), 10, 700, 20, (Color){255, 0, 0, 255});

        EndDrawing();
    }
    CloseWindow();

    pthread_join(threadJouerTour, NULL);

    for (int i = 0; i < nbCasesY; ++i)
    {
        free(points[i]);
    }
    free(points);

	return 0;
}
