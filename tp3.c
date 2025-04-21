#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tp3.h"

// Crée un mot
t_mot *creerMot(char *mot) {
    t_mot *nouveauMot = malloc(sizeof(t_mot));
    if (nouveauMot == NULL) {
        printf("\nErreur Malloc\n");
        return NULL;
    }

    nouveauMot->mot = malloc(strlen(mot) + 1);
    if (nouveauMot->mot == NULL) {
        printf("\nErreur Malloc\n");
        free(nouveauMot);
        return NULL;
    }

    strcpy(nouveauMot->mot, mot);
    nouveauMot->nombre_occurences = 1;
    nouveauMot->suivant = NULL;
    return nouveauMot;
}

// Ajoute un mot dans la liste triée
t_mot *ajouterMot(t_mot *liste, char *mot) {
    if (liste == NULL) {
        return creerMot(mot);
    }

    t_mot *buffer = liste;
    t_mot *prec = NULL;

    while (buffer != NULL && strcmp(buffer->mot, mot) < 0) {
        prec = buffer;
        buffer = buffer->suivant;
    }

    if (buffer != NULL && strcmp(buffer->mot, mot) == 0) {
        buffer->nombre_occurences++;
        return liste;
    }

    t_mot *nouveauMot = creerMot(mot);
    if (nouveauMot == NULL) return liste;

    if (prec == NULL) {
        // Insertion en tête
        nouveauMot->suivant = liste;
        return nouveauMot;
    } else {
        prec->suivant = nouveauMot;
        nouveauMot->suivant = buffer;
        return liste;
    }
}

// Retire un mot
t_mot *retirerMot(t_mot *liste, char *mot) {
    if (liste == NULL) return NULL;

    t_mot *buffer = liste;
    t_mot *prec = NULL;

    while (buffer != NULL && strcmp(buffer->mot, mot) != 0) {
        prec = buffer;
        buffer = buffer->suivant;
    }

    if (buffer == NULL) return liste; // mot pas trouvé

    if (buffer->nombre_occurences > 1) {
        buffer->nombre_occurences--;
        return liste;
    }

    // Supprimer le mot
    if (prec == NULL) {
        liste = buffer->suivant;
    } else {
        prec->suivant = buffer->suivant;
    }
    free(buffer->mot);
    free(buffer);
    return liste;
}

// Affiche tous les mots
void afficherMots(t_mot *liste) {
    if (liste == NULL) {
        printf("Le lexique est vide\n");
        return;
    }

    t_mot *buffer = liste;
    while (buffer != NULL) {
        printf("\n%c --- %s [%d]", toupper(buffer->mot[0]), buffer->mot, buffer->nombre_occurences);
        buffer = buffer->suivant;
    }
    printf("\n");
}

// Fusionne deux listes triées
t_mot *fusionner(t_mot *listeA, t_mot *listeB) {
    if (listeA == NULL) return listeB;
    if (listeB == NULL) return listeA;

    if (strcmp(listeA->mot, listeB->mot) == 0) {
        listeA->nombre_occurences += listeB->nombre_occurences;
        listeA->suivant = fusionner(listeA->suivant, listeB->suivant);
        free(listeB->mot);
        free(listeB);
        return listeA; //si ils sont égaux on renvoie arbitrairement l'élément A et on libere l'elemùent b
    }

    if (strcmp(listeA->mot, listeB->mot) < 0) { //si l'élément A est plus petit que l'element B c'eest à dire que le mot A est placé avant l'élément b dans l'ordre alphabétique on ajoute l'élement A sinon l'inverse
        listeA->suivant = fusionner(listeA->suivant, listeB);
        return listeA;
    } else {
        listeB->suivant = fusionner(listeA, listeB->suivant);
        return listeB;
    }
}

// Importe un fichier texte et ajoute chaque mot
t_mot *importerFichier(t_mot *liste) {
    FILE *f;
    char nomFichier[251];
    printf("\nDonnez le nom du fichier : ");
    scanf("%s", nomFichier); 
    viderBuffer();

    f = fopen(nomFichier, "rt");

    if (f == NULL) {
        printf("\nErreur ouverture fichier\n");
        return liste;
    }

    char *mot;
    while ((mot = lireMot(f)) != NULL) {
        liste = ajouterMot(liste, mot);
        free(mot);
    }

    fclose(f);
    return liste;
}

// Lit un mot depuis un fichier
char *lireMot(FILE *f) {
    int taille = 20;
    int i = 0;
    char *mot = malloc(taille);
    if (!mot) return NULL;

    char c;

    while ((c = fgetc(f)) != EOF && isspace(c));
    if (c == EOF) {
        free(mot);
        return NULL;
    }

    do {
        mot[i++] = c;
        if (i >= taille) {
            taille *= 2;
            char *temp = realloc(mot, taille);
            if (!temp) {
                free(mot);
                return NULL;
            }
            mot = temp;
        }
        c = fgetc(f);
    } while (c != EOF && !isspace(c));

    mot[i] = '\0';
    return convertir_minuscule(mot);
}

// Convertit un mot en minuscules
char *convertir_minuscule(char *mot) {
    for (int i = 0; mot[i] != '\0'; i++) {
        mot[i] = tolower(mot[i]);
    }
    return mot;
}

// Vide le buffer clavier
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
