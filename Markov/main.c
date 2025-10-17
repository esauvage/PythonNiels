/*
 * Markov.c
 *
 * Copyright 2025 Niels Sauvage <niels@les-sauvages.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct dico {
    char cle;
    char valeur[1024];
} Dico;

Dico monDico[2048];

void pretraiter(char * texte, char * sortie)
{
    // Transforme le texte donné en entrée en supprimant tous les caractères non-lettre
    // et en mettant les lettres en majuscules.

	int index = 0;
	for (int i = 0; texte[i]; ++i)
	{
		if (isalpha(texte[i]))
		{
			sortie[index] = toupper(texte[i]);
			++index;
		}
	}
}

int lenFile(void) {
    // Fonction servant à obtenir le nombre de caractère d'un fichier texte

    FILE *fp = fopen("texte.txt", "r");

    if (!fp) return -1;

    int c, lFile = 0;
    while ((c = getc(fp)) != EOF) lFile++;

    fclose(fp);

    return lFile;
}
/*
 * Ne sert à rien (je crois).
 * La fonction recupererMots() fonctionne mieux.
 */
// char *readFile(char *texte) {
//     FILE *fp = fopen("texte.txt", "r");

//     if (!fp) return NULL;

//     int c, i = 0;
//     while ((c = getc(fp)) != EOF) {
//         if (c != '\n') texte[i++] = (char)c;
//     }
//     texte[i] = '\0';
//     fclose(fp);
//     return texte;
// }

void analyser(char *texte) {
    /*
     * Sert à construire le tableau de structures "monDico".
     */

    int index = 0;

    // Jusqu'à l'avant dernier caractères
    for (int i = 0; texte[i+1]; i++) {
        char courant = texte[i];
        char suivant = texte[i+1];
        int trouve = 0;

        // Pour tous les structures du tableau
        for (int x = 0; monDico[x].cle; x++) {
            // Si notre lettre est trouvé, on rajoute celle qui vient après
            if (monDico[x].cle == courant) {
                int len = strlen(monDico[x].valeur);
                monDico[x].valeur[len] = suivant;
                monDico[x].valeur[len+1] = '\0';

                // On a trouvé notre lettre
                trouve = 1;
                break;
            }
        }

        // Si on a pas trouvé notre lettre dans la boucle plus haut
        if (!trouve) {
            monDico[index].cle = courant;
            monDico[index].valeur[0] = suivant;
            monDico[index].valeur[1] = '\0';
            index++;
        }
    }
}

char * genererMot(char * mot, int longueurMot)
{
	int premiereLettre;
	int suivante;
	int nbKeys;
	int nbValues;
	//int indexLettre;
	int indexCle;
	int i;

	// Obtenir le nombre de clés du dico
	for (nbKeys = 0; monDico[nbKeys].cle; ++nbKeys) {}

	// Prendre une première lettre au pif et la mettre en 1ère position
	premiereLettre = rand() % nbKeys;
	mot[0] = monDico[premiereLettre].cle;

	// 1 parce que l'on a déjà la 1ère lettre
	i = 1;
	while (i < longueurMot)
	{
		char *tmp = monDico[premiereLettre].valeur;

		// Obtenir le nombre de lettres qui suivent la dernière lettre
		// Obsolète
		// for (nbValues = 0; *tmp; ++nbValues, tmp++) {}
		// peut être remplacer par
		nbValues = strlen(tmp);

		// On choisit au pif la lettre suivante parmi celles disponibles
		suivante = rand() % nbValues;
		mot[i] = monDico[premiereLettre].valeur[suivante];

		// Trouver l'index de la lettre suivante dans les clés. (pas très clair)
		for (indexCle = 0; monDico[indexCle].cle != monDico[premiereLettre].valeur[suivante]; ++indexCle) {}
		suivante = indexCle;
		premiereLettre = suivante;

		++i;
	}
	mot[i] = '\0';

	return mot;
}

void recupererMots(char * texte)
{
    FILE *fp = fopen("texte.txt", "r");
    if (!fp) return;

    int index = 0;
    int carac;

    while ((carac = fgetc(fp)) != EOF)
    {
        texte[index] = carac;
        index++;
    }
    texte[index] = '\0';

    fclose(fp);
}

int main(void) {
    //~ int lFile = lenFile();
    //~ if (lFile <= 0) return 1;

	srand(time(NULL));

	int longueurMot;

    //~ char *texte = malloc(lFile + 1);
    //~ readFile(texte);
    // char texte[] = "Gwendalavir Ewilan Gil'Sayan Salim Condo Edwin Til'Ilan Ellana Caldin Bjorn Wil'Wayard Maniel Hans Duom Nil'Erg Chiam Vite Ellundril Chariakin Merwyn Ril'Avalon Isaya Caldin Sil'Afian Altan Gil'Sayan Elicia Gil'Sayan Ilian";

    char texte[1000];

    recupererMots(texte);

    printf("%s\n", texte);

	//char *texteMaj = malloc(lFile + 1);
	char * texteMaj = malloc(strlen(texte));
	pretraiter(texte, texteMaj);

	printf("%s\n", texteMaj);
	//strcpy(texteMaj, pretraiter(texte, texteMaj));
	//char * texteMaj = strdup(pretraiter(texte, texteMaj));
    analyser(texteMaj);

    for (int x = 0; monDico[x].cle; x++) {
        //printf("{\"%c\" : \"%s\"}\n", monDico[x].cle, monDico[x].valeur);
    }

    char mot[9];
	for (int i = 0; i < 100; ++i)
    {
		longueurMot = rand() % 9;
		if (longueurMot <= 4) {longueurMot += 4;}

		strcpy(mot, genererMot(mot, longueurMot));

		printf("%s\n", mot);

	}
    //~ free(texte);
    free(texteMaj);
    return 0;
}
