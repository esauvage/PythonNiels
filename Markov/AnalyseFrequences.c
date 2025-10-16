#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
}

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

int estDansSecondaire(Secondaire * frequences, char lettre, int * index)
{
    for (int i = 0; frequences[i].lettre; ++i)
    {
        if (lettre == frequences[i].lettre)
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
        /*
         * Si la lettre courante est déjà dans la structure principale, on vérifie si la lettre suivante
         * (celle que l'on veut écrire dans la structure secondaire de la lettre courante) existe, si oui,
         * on l'incrémente de 1, sinon on la créé et on l'incrémente de 1. Si la lettre courante
         * n'est pas dans la structure principale, on l'ajoute et on ajoute aussi la lettre suivante
         * dans la structure secondaire.
         */
        if (estDansDico(texte[i], &indexDico))
        {
            int pos = 0;

            if (estDansSecondaire(frequences[indexDico].secondaire, texte[i + 1], &pos))
            {
                frequences[indexDico].secondaire[pos].nbOccurences++;
            }
            else
            {
                frequences[indexDico].secondaire[frequences[indexDico].indexSecondaire].lettre = texte[i + 1];
                frequences[indexDico].secondaire[frequences[indexDico].indexSecondaire].nbOccurences++;

                frequences[indexDico].indexSecondaire++;
            }
        }
        else
        {
            frequences[index].lettre = texte[i];
            frequences[index].secondaire[frequences[index].indexSecondaire].lettre = texte[i + 1];
            frequences[index].secondaire[frequences[index].indexSecondaire].nbOccurences += 1;

            frequences[index].indexSecondaire++;
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
        frequences[i].lettre = 0;
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

void recupererMots(char * texte)
{
    FILE *fp = fopen("texte2.txt", "r");
    if (!fp) return;

    int index = 0;
    int carac;

    while ((carac = fgetc(fp)) != EOF)
    {
        if (carac != '\n')
        {
            texte[index] = carac;
            index++;
        }
    }

    texte[index] = '\0';

    fclose(fp);
}

int main(int argc, char ** argv)
{
    char texte[1000];
    // char texteP[1000];

    recupererMots(texte);

    // init(texte, texteP);
    init(texte);

    construireFrequences(texteP);
    afficherStructure();

    return 0;
}
