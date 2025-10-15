#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct dico2
{
    char lettre;
    int nbOccurences;
} Secondaire;

typedef struct dico
{
    char lettre;
    int indexSecondaire;
    Secondaire secondaire[1000];
} Principale;

Principale frequences[1000];
char texteP[1000];

int estDansDico(char lettre, int * index)
{
    /*
     * Sert à vérifier si une lettre donné en paramètre est déjà
     * dans le tableau des frequences. Si oui, retourne 1,
     * sinon, retourne 0. Si on précise en paramètre que l'on veut
     * aussi l'index de la lettre, on met cet index dans un pointeur.
     */
    for (int i = 0; frequences[i].lettre; ++i)
    {
        // printf("%c\n", frequences[i].lettre);
        if (frequences[i].lettre == lettre)
        {
            if (index != NULL)
            {
                (*index) = i;
            }

            return 1;
        }
    }
    return 0;
}

void construireFrequences(char * texte)
{
    int index = 0;
    int indexDico = 0;

    for (int i = 0; texte[i + 1]; ++i)
    {
        if (estDansDico(texte[i], &indexDico))
        {
            //puts("Oui");
            // frequences[i].secondaire[frequences[i].indexSecondaire].lettre = texte[i + 1];
            // frequences[i].secondaire[frequences[i].indexSecondaire].nbOccurences += 1;

            // frequences[i].indexSecondaire++;
            frequences[indexDico].secondaire[frequences[indexDico].indexSecondaire].lettre = texte[i + 1];
            frequences[indexDico].secondaire[frequences[indexDico].indexSecondaire].nbOccurences += 1;

            frequences[indexDico].indexSecondaire++;
            //index++;
        }
        else
        {
            frequences[index].lettre = texte[i];
            frequences[index].secondaire[frequences[index].indexSecondaire].lettre = texte[i + 1];
            frequences[index].secondaire[frequences[index].indexSecondaire].nbOccurences += 1;

            ++index;
        }
    }
}

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

void init(char * texte)
{
    /*
     * Initialisation.
     * Important car sinon la fonction estDansDico() ne fonctionne pas
     */
    for (int i = 0; frequences[i].lettre; ++i)
    {
        frequences[i].lettre = ' ';
        frequences[i].indexSecondaire = 0;

        /*
         * Initialisation des variables de secondaire
         */
         for (int x = 0; frequences[i].secondaire[x].nbOccurences; ++x)
         {
             frequences[i].secondaire[x].nbOccurences = 0;
         }
    }

    /*
     * Prétraitement du texte
     */
     pretraiter(texte, texteP);
}

void afficherStructure()
{
    putchar('{');
    for (int i = 0; frequences[i].lettre; ++i)
    {
        if (i)
        {
            printf("}, ");
        }

        printf("'%c' : {", frequences[i].lettre);

        for (int x = 0; frequences[i].secondaire[x].lettre; ++x)
        {
            if (x)
            {
                printf(", ");
            }
            printf("'%c' : %d", frequences[i].secondaire[x].lettre, frequences[i].secondaire[x].nbOccurences);
        }
    }
    printf("}}\n");
    // putchar('\n');
}

int main(int argc, char ** argv)
{
    // frequences[0].lettre = 'T';

    if (argc > 1)
    {
        init(argv[1]);
        construireFrequences(texteP);
    }

    afficherStructure();

    return 0;
}
