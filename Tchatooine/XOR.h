#ifndef XOR_H
#define XOR_H

#include "sha256.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int ChiffrerFichier(const char * nomFichier, const char * output, const char * cle);
int ChiffrerTexte(const char * msg, char * output, const char * cle);

#endif
