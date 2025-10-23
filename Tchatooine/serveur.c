/**
 * @file serveur.c
 * @brief Serveur de tchat multi-clients utilisant les threads POSIX.
 * @version 1.1
 * @date 2025-10-22
 * @author Luke
 *
 * Ce programme met en place un serveur TCP simple capable de gérer plusieurs
 * clients simultanément grâce aux threads. Chaque client reçoit les messages
 * envoyés par les autres, dans une ambiance conviviale et codée à la main.
 *
 *  Conception :
 *  - Chaque client est géré dans un thread distinct.
 *  - La liste des clients et pseudos est globale et protégée par la logique
 *    (pas encore de mutex, mais t’inquiète, un jour).
 *  - Tout message est rediffusé à tous les clients.
 *
 *  Citation du codeur :
 *  “Un sleep est un pansement, pas une solution.”
 */

#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit(), EXIT_FAILURE, malloc(), free()
#include <string.h>     // strlen(), memset(), strcpy(), strcmp()
#include <unistd.h>     // close()
#include <sys/socket.h> // socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h> // struct sockaddr_in, htons(), INADDR_ANY
#include <pthread.h>    // pthread_t, pthread_create(), pthread_detach()
#include "XOR.h"        // ChiffrerTexte()

#define PORT 8080 ///< Le port sur lequel le serveur écoutera
#define MAX_CLIENTS 1024 ///< Nombre maximal de clients simultanés

int clients[MAX_CLIENTS];        ///< Tableau des sockets clients connectés
char pseudos[MAX_CLIENTS][50];   ///< Tableau des pseudos associés aux clients
int nbClients = 0;               ///< Nombre actuel de clients connectés

const char * cle = "PèreNoël256";

/**
 * @struct InfoClient
 * @brief Contient les informations associées à un client.
 *
 * Utilisé pour gérer les threads client.
 */
typedef struct infoClient
{
    int client;     /**< Socket du client */
    char *pseudo;   /**< Pseudo du client (copie dynamique) */
} InfoClient;


/**
 * @brief Diffuse un message à tous les clients connectés.
 *
 * @param message Le texte à diffuser (terminé par '\0')
 */
void diffuser(const char *message)
{
    for (int i = 0; i < nbClients; ++i)
    {
        if (clients[i] != 0)
            send(clients[i], message, strlen(message) + 1, 0);
    }
}

/**
 * @brief Déconnecte proprement un client et informe les autres.
 *
 * @param client Le socket du client à fermer
 * @param pseudo Le pseudo du client qui quitte
 */
void deconnexionClient(int client, const char *pseudo)
{
    int index = -1;

    for (int i = 0; i < nbClients; ++i)
    {
        if (!strcmp(pseudos[i], pseudo))
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        printf("%s a quitté le tchat\n", pseudo);
        close(client);
        clients[index] = 0;
        memset(pseudos[index], 0, sizeof(pseudos[index]));

        char msg[1024];
        sprintf(msg, "%s a quitté le tchat", pseudo);
        diffuser(msg);
    }
}

/**
 * @brief Gère la communication avec un client.
 *
 * Cette fonction tourne dans un thread dédié pour chaque client.
 *
 * @param arg Pointeur vers une structure InfoClient (à libérer à la fin)
 * @return NULL
 */
void * gestionClient(void *arg)
{
    InfoClient *monClient = (InfoClient *)arg;
    int client = monClient->client;
    char pseudo[50];
    strcpy(pseudo, monClient->pseudo);

    free(monClient->pseudo);
    free(monClient);

    char msg[1024];
    char msgDechiffre[1024];

    while (1)
    {
        ssize_t valread = recv(client, msg, sizeof(msg), 0);

        // Si le client a quitté ou erreur
        if (valread <= 0)
        {
            deconnexionClient(client, pseudo);
            return NULL;
        }

        ChiffrerTexte(msg, msgDechiffre, cle);

        // msgDechiffre[valread] = '\0'; // Sécurise la fin de chaîne

        char test[1024];
        sprintf(test, "%s : exit", pseudo);
        if (!strcmp(msgDechiffre, test))
        {
            deconnexionClient(client, pseudo);
            return NULL;
        }

        // Rediffuse à tous les autres
        printf("[%s] %s\n", pseudo, msgDechiffre);
        diffuser(msg);
    }

    return NULL;
}

/**
 * @brief Accepte les nouvelles connexions et lance un thread par client.
 *
 * @param socket_serveur Socket du serveur
 * @param adresse Structure d’adresse
 * @param taille_adresse Taille de la structure
 */
void gestionConnexion(int socket_serveur, struct sockaddr_in adresse, socklen_t taille_adresse)
{
    while (1)
    {
        int socket_client = accept(socket_serveur, (struct sockaddr *)&adresse, &taille_adresse);
        if (socket_client < 0)
        {
            perror("Erreur : accept()");
            continue;
        }

        puts("Un client s'est connecté !");

        char pseudo[1024];
        char pseudoDechiffre[1024];
        recv(socket_client, pseudo, sizeof(pseudo), 0);

        ChiffrerTexte(pseudo, pseudoDechiffre, "PèreNoël256");

        // Enregistre le nouveau client
        clients[nbClients] = socket_client;
        strcpy(pseudos[nbClients], pseudo);
        nbClients++;

        printf("%s a rejoint le tchat\n", pseudo);

        char msg[1024];
        sprintf(msg, "%s a rejoint le tchat", pseudo);
        diffuser(msg);

        InfoClient *infoClient = malloc(sizeof(InfoClient));
        infoClient->client = socket_client;
        infoClient->pseudo = strdup(pseudo);

        pthread_t t_client;
        pthread_create(&t_client, NULL, gestionClient, infoClient);
        pthread_detach(t_client); // Le thread s’auto-libère à la fin
    }
}

/**
 * @brief Point d'entrée du serveur.
 */
int main()
{
    int serveur_fd;
    struct sockaddr_in adresse;
    socklen_t taille_adresse = sizeof(adresse);

    // Initialisation des tableaux
    memset(clients, 0, sizeof(clients));
    memset(pseudos, 0, sizeof(pseudos));

    // --- Création du socket ---
    serveur_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serveur_fd < 0)
    {
        perror("Erreur : socket()");
        exit(EXIT_FAILURE);
    }

    // --- Configuration du socket ---
    int option = 1;
    if (setsockopt(serveur_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) < 0)
    {
        perror("Erreur : setsockopt()");
        exit(EXIT_FAILURE);
    }

    // --- Remplissage de la structure sockaddr_in ---
    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = INADDR_ANY;
    adresse.sin_port = htons(PORT);

    // --- Association du socket à l’adresse ---
    if (bind(serveur_fd, (struct sockaddr *)&adresse, sizeof(adresse)) < 0)
    {
        perror("Erreur : bind()");
        exit(EXIT_FAILURE);
    }

    // --- Mise en écoute ---
    if (listen(serveur_fd, 10) < 0)
    {
        perror("Erreur : listen()");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion sur le port %d...\n", PORT);

    // --- Boucle principale ---
    gestionConnexion(serveur_fd, adresse, taille_adresse);

    // --- Fermeture ---
    close(serveur_fd);
    printf("Serveur fermé proprement.\n");

    return 0;
}
