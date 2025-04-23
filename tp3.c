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
    nettoyerMot(mot);
    convertir_minuscule(mot);
    if (strlen(mot) == 0) return liste;
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
    nettoyerMot(mot);
    convertir_minuscule(mot);
    if (strlen(mot) == 0) {
        printf("Mot invalide.\n");
        return liste;
    }

    if (liste == NULL) {
        printf("La liste est vide.\n");
        return NULL;
    }

    t_mot *buffer = liste;
    t_mot *prec = NULL;

    while (buffer != NULL && strcmp(buffer->mot, mot) != 0) {
        prec = buffer;
        buffer = buffer->suivant;
    }

    if (buffer == NULL) {
        printf("Mot \"%s\" n'est pas dans la liste.\n", mot);
        return liste;
    }

    if (buffer->nombre_occurences > 1) {
        buffer->nombre_occurences--;
    } else {
        if (prec == NULL) {
            liste = buffer->suivant;
        } else {
            prec->suivant = buffer->suivant;
        }
        free(buffer->mot);
        free(buffer);
    }

    printf("Mot \"%s\" supprimé.\n", mot);
    return liste;
}
// afficher mots
void afficherMots(t_mot *liste){
    if (liste == NULL) {
        printf("Le lexique est vide.\n");
        return;
    }
    char lettre = '0';
    while (liste != NULL) {
        if (liste->mot[0] != lettre) {
            lettre = liste->mot[0];
            printf("%c ", toupper(lettre));
        }
        else printf("  ");
        printf("--- %s[%d]\n", liste->mot, liste->nombre_occurences);
        liste = liste->suivant;
    }
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
        return listeA;
    }

    if (strcmp(listeA->mot, listeB->mot) < 0) {
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

    while (c != EOF && !isspace(c)) {
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
    } 

    mot[i] = '\0';
    return mot;
}

// Convertit un mot en minuscules
char *convertir_minuscule(char *mot) {
    for (int i = 0; mot[i] != '\0'; i++) {
        mot[i] = tolower((unsigned char)mot[i]);
    }
    return mot;
}

// Vide le buffer clavier
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char *nettoyerMot(char *mot) {
    int i = 0, j = 0;
    unsigned char c;
    
    while (mot[i] != '\0') {
        c = (unsigned char)mot[i];
        if (isalpha(c) || (c >= 192 && c <= 255)) {  
            mot[j++] = mot[i];  
        }
        i++;
    }
    mot[j] = '\0';
    return mot;
}


void libererLexique(t_mot *liste) {
    t_mot *tmp;
    while (liste != NULL) {
        tmp = liste;
        liste = liste->suivant;
        free(tmp->mot);
        free(tmp);
    }
}

void libererTousLexiques(t_mot **lexiques, int nbLexiques) {
    for (int i = 0; i < nbLexiques; i++) {
        libererLexique(lexiques[i]);
    }
    free(lexiques);
}

int choisirLexique(int nbLexiques) {
    int choix=0;   // erreur oublie de remetre l'initialisation de choix à 0 qui fait que la fonction choix ne fonctionne pas int choix=0; normalement
    while (choix < 1 || choix > nbLexiques) {
        printf("\nChoisissez le lexique (1 à %d) : ", nbLexiques);
        scanf("%d", &choix);
        viderBuffer();
        if (choix < 1 || choix > nbLexiques) {
            printf("Choix invalide : '%d'. Veuillez entrer un nombre entre 1 et %d.\n", choix, nbLexiques);
        }
    }
    return choix - 1; 
}
