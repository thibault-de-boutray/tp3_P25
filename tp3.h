#ifndef TP3_H
#define TP3_H

#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>  
#include <ctype.h>   

typedef struct t_mot {
    char *mot;
    int nombre_occurences;
    struct t_mot *suivant;
} t_mot;

t_mot *creerMot(char *mot);
t_mot *ajouterMot(t_mot *liste, char *mot);
t_mot *retirerMot(t_mot *liste, char *mot);
void afficherMots(t_mot *liste);
t_mot *fusionner(t_mot *listeA, t_mot *listeB);
t_mot *importerFichier(t_mot *liste);
char *lireMot(FILE *f);
char *convertir_minuscule(char *mot);
void viderBuffer(void);

#endif
