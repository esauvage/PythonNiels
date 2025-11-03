#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
    int couleur;
} Point;

int lancerThread = 1;
int delai = 100;
int generation = 0;

int lireConfig(Point ** points)
{
    // int indexPoints = 0;
    int indexNb = 0;
    char xOuY = 'x';

    FILE * fichier = fopen("config.txt", "r");
    if (!fichier) {return 1; puts("Erreur (l. 22)");}

    char data[108 * 72];

    ssize_t nbLues = fread(data, sizeof(char), 108 * 72, fichier);
    data[nbLues] = '\0';

    char nbX[5];
    nbX[0] = '\0';

    char nbY[5];
    nbY[0] = '\0';
    for (int i = 0; i < (int)strlen(data); ++i)
    {
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

            if (xOuY == 'y') {xOuY = 'x'; points[atoi(nbY)][atoi(nbX)].couleur = 1;}
            else if (xOuY == 'x') xOuY = 'y';
        }
    }

    fclose(fichier);
    return 1;
}

void dessinerGrille()
{
   	for (int i = 10; i < 1080; i += 10)
	{
        DrawLine(i, 0, i, 720, BLACK);
	}
	for (int i = 10; i < 720; i += 10)
	{
        DrawLine(0, i, 1080, i, BLACK);
	}
}

void dessinerPointsNoirs(Point ** points)
{
    for (int y = 0; y < 72; ++y)
	{
        for (int x = 0; x < 108; ++x)
		{
		    if (points[y][x].couleur == 1) DrawRectangle(x * 10, y * 10, 10, 10, BLACK);
		}
	}
}

void copier(Point ** points, Point ** pointsTemp)
{
    for (int y = 0; y < 72; ++y)
    {
        for (int x = 0; x < 108; ++x)
        {
            pointsTemp[y][x] = points[y][x];
        }
    }
}

void * jouerTour(void * arg)
{
    Point ** points = (Point **)arg;
    Point ** pointsTemp;

    pointsTemp = malloc(72 * sizeof(Point *));
    if (!pointsTemp) puts("Erreur malloc()");

    for (int i = 0; i < 72; ++i)
    {
        pointsTemp[i] = malloc(108 * sizeof(Point));
        if (!pointsTemp) puts("Erreur malloc()");
    }

    if (generation == 0)
    {
        usleep(delai * 1000);
    }

    copier(points, pointsTemp);

    for (int y = 0; y < 72; y++)
    {
        for (int x = 0; x < 108; x++)
        {
            int nbCasesAutour = 0;

            // La case au dessus
            if (y > 0 && points[y - 1][x].couleur == 1) nbCasesAutour++;
            // La case au dessous
            if (y < 71 && points[y + 1][x].couleur == 1) nbCasesAutour++;
            // La case à gauche
            if (x > 0 && points[y][x - 1].couleur == 1) nbCasesAutour++;
            // La case au droite
            if (x < 108 && points[y][x + 1].couleur == 1) nbCasesAutour++;
            // La case en haut à gauche
            if (y > 0 && x > 0 && points[y - 1][x - 1].couleur == 1) nbCasesAutour++;
            // La case en haut à droite
            if (y > 0 && x < 108 && points[y - 1][x + 1].couleur == 1) nbCasesAutour++;
            // La case en bas à gauche
            if (y < 71 && x > 0 && points[y + 1][x - 1].couleur == 1) nbCasesAutour++;
            // La case en bas à droite
            if (y < 71 && x < 108 && points[y + 1][x + 1].couleur == 1) nbCasesAutour++;

            if (nbCasesAutour == 3 && points[y][x].couleur == 0) pointsTemp[y][x].couleur = 1;
            if ((nbCasesAutour != 2 && nbCasesAutour != 3) && points[y][x].couleur == 1) pointsTemp[y][x].couleur = 0;
        }
    }

    copier(pointsTemp, points);

    for (int i = 0; i < 72; ++i)
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

void initPoints(Point ** points)
{
    for (int y = 0; y < 72; y++)
    {
        for (int x = 0; x < 108; x++)
        {
            points[y][x].couleur = 0;
        }
    }
}

int main()
{
    InitWindow(1080, 720, "Game of life");
    SetTargetFPS(60);

    // Point points[720][1080];
    Point ** points;

    points = malloc(72 * sizeof(Point *));
    for (int i = 0; i < 72; ++i)
    {
        points[i] = malloc(108 * sizeof(Point));
    }

    initPoints(points);

    int o = lireConfig(points);
    if (!o) {return 1; puts("Erreur");}

    pthread_t threadJouerTour;

    while (!WindowShouldClose())
    {
        if (lancerThread)
        {
            pthread_join(threadJouerTour, NULL);
            pthread_create(&threadJouerTour, NULL, jouerTour, points);
            lancerThread = 0;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        dessinerGrille();
        dessinerPointsNoirs(points);

        DrawText(TextFormat("Génération : %d", generation), 10, 700, 20, BLACK);

        EndDrawing();
    }
    CloseWindow();

    pthread_join(threadJouerTour, NULL);

    for (int i = 0; i < 72; ++i)
    {
        free(points[i]);
    }
    free(points);

	return 0;
}
