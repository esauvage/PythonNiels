#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct dico2
{
    char lettre;
    int nbOccurences;
} Secondaire;

typedef struct dico
{
    char lettre;
    int indexSecondaire;
    int nbOccurences;
    Secondaire secondaire[1000];
} Principale;

void afficherStructure(Principale * frequences)
{
    putchar('{');
    for (int i = 0; frequences[i].lettre; ++i)
    {
        if (i) printf("}, ");
        printf("'%c' : {", frequences[i].lettre);

        for (int x = 0; frequences[i].secondaire[x].lettre; ++x)
        {
            if (x) printf(", ");
            printf("'%c' : %d", frequences[i].secondaire[x].lettre, frequences[i].secondaire[x].nbOccurences);
        }
    }
    printf("}}\n");
}

int estDansDico(char lettre, int * index, Principale * frequences)
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

void construireFrequences(char * texte, Principale * frequences)
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
        if (estDansDico(texte[i], &indexDico, frequences))
        {
            int pos = 0;

            frequences[index].nbOccurences++;

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
            frequences[index].nbOccurences++;
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
		if (isalpha(texte[i]) || texte[i] == ' ')
		{
			sortie[index] = toupper(texte[i]);
			++index;
		}
	}
	sortie[index] = 0;
}

void init(char * texte, Principale * frequences)
{
    /*
    * Initialisation.
    * Important car sinon la fonction estDansDico() ne fonctionne pas
    */
    for (int i = 0; i < 1000; ++i)
    {
        frequences[i].lettre = 0;
        frequences[i].indexSecondaire = 0;

        for (int x = 0; x < 1000; ++x)
        {
            frequences[i].secondaire[x].lettre = 0;
            frequences[i].secondaire[x].nbOccurences = 0;
        }
    }

    pretraiter(texte, texte);
}

void recupererMots(char * texte, char * nomFichier)
{
    FILE *fp = fopen(nomFichier, "r");
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

void longueurFichier(long int * taille, char * nomFichier)
{
    FILE *fp = fopen(nomFichier, "r");

    fseek(fp, 0, SEEK_END);

    (*taille) = ftell(fp);

    fclose(fp);
}

void listerLettres(Principale * frequences, char * lettres)
{
    int i = 0;
    for (i = 0; frequences[i].lettre; ++i)
    {
        // putchar(frequences[i].lettre);
        lettres[i] = frequences[i].lettre;
    }

    // putchar('\n');
    lettres[i] = 0;
}

void nbLettresDansSecondaire(Secondaire * frequences, int * nbLettres)
{
    for (int i = 0; frequences[i].lettre; ++i)
    {
        (*nbLettres)++;
        // printf("%d\n", (*nbLettres));
    }
}

int nbLettres(Principale *p) {
    int ret = 0;

    for (int i = 0; p[i].lettre; ++i) ret+= p[i].nbOccurences;

    return ret;
}

void genererMots(Principale *p) {
    //On va être des oufs : on va initialiser à la fois la dernière lettre possible et le nombre de lettres lu.
    int lettreCourante = 0;
    int taille = 0;

    for (; p[lettreCourante].lettre; ++lettreCourante) taille += p[lettreCourante].nbOccurences;
    //lettreCourante pointe maintenant sur la dernière lettre de Principale *p.

    //on tire entre 0 et taille-1
    int debut = rand() % taille;

    //On soustrait à ce tirage le nombre d'occurences de chaque lettre de p, jusqu'à passer sous 0.
    //Au pire, on s'arrête sur la dernière lettre
    for (int i = 0; lettreCourante > i; ++i) {
        debut -= p[i].nbOccurences;
        lettreCourante = debut > 0 ? lettreCourante : i;
    }
    printf("%c", p[lettreCourante].lettre);

    // for (int x = 0; x < longueur; ++x)
    while (p[lettreCourante].lettre != ' ')
    {
        int nbLettresSecond = 0;

        for (int i = 0; p[lettreCourante].secondaire[i].lettre; ++i) nbLettresSecond += p[lettreCourante].secondaire[i].nbOccurences;

        int choix = rand() % nbLettresSecond;

        int i;
        for (i = 0; choix > 0; ++i)
        {
            choix -= p[lettreCourante].secondaire[i].nbOccurences;
        }
        printf("%c", p[lettreCourante].secondaire[i].lettre);

        int indexSuiv;
        estDansDico(p[lettreCourante].secondaire[i].lettre, &indexSuiv, p);

        lettreCourante = indexSuiv;
    }
    putchar('\n');
}

int main(int argc, char ** argv)
{
    srand(time(NULL));

    char * texte;
    Principale frequences[1000];

    if (argc > 1)
    {
        long int taille;
        longueurFichier(&taille, argv[1]);

        texte = (char *)malloc(taille + 1 * sizeof(char));
        recupererMots(texte, argv[1]);

        // printf("%ld\n", taille);

        init(texte, frequences);

        construireFrequences(texte, frequences);
        // afficherStructure(frequences);

        for (int i = 0; i < 100; ++i)
        {
            genererMots(frequences);
        }

        free(texte);
    }

    return 0;
}
