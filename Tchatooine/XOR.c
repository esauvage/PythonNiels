#include "XOR.h"

int ChiffrerTexte(const char * msg, char * output, const char * cle)
{
    if (!msg) return 0;

    BYTE CleSHA256[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    strcpy((char *)CleSHA256, cle);

    // SHA-256 de la clé
    sha256_init(&ctx);
    sha256_update(&ctx, (BYTE *)cle, strlen(cle));
    sha256_final(&ctx, CleSHA256);

    // XOR du message
    int i;
    for (i = 0; msg[i]; ++i)
    {
        output[i] = msg[i] ^ CleSHA256[i % SHA256_BLOCK_SIZE];
    }
    output[i] = '\0';

    return 1;
}

int ChiffrerFichier(const char * nomFichier, const char * output, const char * cle)
{
    BYTE CleSHA256[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    strcpy((char *)CleSHA256, cle);

    // SHA-256
    sha256_init(&ctx);
    sha256_update(&ctx, (BYTE *)cle, strlen(cle));
    sha256_final(&ctx, CleSHA256);

    FILE *f = fopen(nomFichier, "rb");
    if (!f) {
        perror("fopen entrée");
        return 0;
    }

    // obtenir la taille
    if (fseek(f, 0, SEEK_END) != 0) { perror("fseek"); fclose(f); return 0; }
    long size = ftell(f);
    if (size < 0) { perror("ftell"); fclose(f); return 0; }
    rewind(f);

    // allouer et lire
    unsigned char *buf = malloc(size);
    if (!buf) { perror("malloc entrée"); fclose(f); return 0; }
    if (fread(buf, sizeof(char), size, f) != (size_t)size) {
        perror("fread"); free(buf); fclose(f); return 0;
    }

    fclose(f);

    f = fopen(output, "wb");
    if (!f) {
        perror("fopen sortie"); free(buf); return 0;
    }

    unsigned char *result = malloc(size);
    if (!result) { perror("malloc sortie"); free(buf); fclose(f); return 0; }

    // ============ Le XOR ============
    for (int i = 0; i < size; ++i) {
        result[i] = buf[i] ^ CleSHA256[i % SHA256_BLOCK_SIZE];
    }

    if (fwrite(result, sizeof(char), size, f) != (size_t)size) {
        perror("fwrite"); free(buf); free(result); fclose(f); return 0;
    }

    free(buf);
    free(result);
    fclose(f);

    return 1; // tout s'est bien passé
}

// int main(int argc, char ** argv)
// {
//     if (argc == 4)
//     {
//         Chiffrer(argv[1], argv[2], argv[3]);
//     }
//     else
//     {
//         puts("Usage : ./XOR <fichier_entree> <fichier_sortie> <cle>");
//         puts("Exemple : ./XOR secret.txt secret.xor motdepasse");
//     }

//     return 0;
// }
