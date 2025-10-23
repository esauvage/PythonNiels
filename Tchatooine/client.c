#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "XOR.h"

#define PORT 8080

int envoyer_actif = 1;
char pseudo[1024];
char pseudoChiffre[1024];

const char * cle = "PèreNoël256";

void * envoyerMessage(void * arg)
{
    int socket_client = *(int *)arg;

    while (1)
    {
        char msg_f[1024];
        char msg[1024];
        char msgChiffre[1024];

        fgets(msg, sizeof(msg), stdin);
        sprintf(msg_f, "%s : %s", pseudo, msg);
        msg_f[strlen(msg_f) - 1] = '\0';

        ChiffrerTexte(msg_f, msgChiffre, cle);

        send(socket_client, msgChiffre, sizeof(msgChiffre), 0);

        // Si l'utilisateur veut quitter
        char test[1024];
        sprintf(test, "%s : exit", pseudo);
        if (!strcmp(msg_f, test))
        {
            envoyer_actif = 0;
            break;
        }
    }
    return NULL;
}

void recevoirMessage(int socket_client)
{
    pthread_t t_send;
    pthread_create(&t_send, NULL, envoyerMessage, &socket_client);

    puts("Bienvenue dans le tchat !");

    while (1)
    {
        char buf[1024];
        char msgDechiffre[1024];
        int valread;

        valread = recv(socket_client, buf, sizeof(buf), 0);

        if (!valread) break;
        if (!envoyer_actif) break;

        ChiffrerTexte(buf, msgDechiffre, cle);

        puts(msgDechiffre);
    }
    pthread_cancel(t_send); // arrête le thread d'envoi
}

int main(int argc, char const* argv[])
{
    int status, client_fd;
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Pseudo : ");
    fgets(pseudo, sizeof(pseudo), stdin);

    pseudo[strlen(pseudo) - 1] = '\0'; // Supprimer le '\n' à la fin du message

    ChiffrerTexte(pseudo, pseudoChiffre, cle);

    send(client_fd, pseudoChiffre, sizeof(pseudoChiffre), 0);

    recevoirMessage(client_fd);

    close(client_fd);
    return 0;
}
