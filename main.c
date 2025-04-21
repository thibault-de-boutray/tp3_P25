#include "tp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void libererLexique(t_mot *liste) {
    t_mot *tmp;
    while (liste != NULL) {
        tmp = liste;
        liste = liste->suivant;
        free(tmp->mot);
        free(tmp);
    }
}

char *nettoyerMot(char *mot) {
    int i = 0, j = 0;
    while (mot[i] != '\0') {
        if (isalpha((unsigned char)mot[i])) {
            mot[j++] = mot[i];
        }
        i++;
    }
    mot[j] = '\0';
    return mot;
}

int choisirLexique() {
    int choix;
    do {
        printf("\nChoisissez le lexique (1 ou 2) : ");
        scanf("%d", &choix);
        viderBuffer();
    } while (choix != 1 && choix != 2);
    return choix;
}

int main() {
    char choixMenu = '0';
    t_mot *lexique1 = NULL;
    t_mot *lexique2 = NULL;

    do {
        printf("\n========================================");
        printf("\n  [1] Afficher un lexique");
        printf("\n  [2] Ajouter un mot dans un lexique");
        printf("\n  [3] Retirer un mot d'un lexique");
        printf("\n  [4] Fusionner deux lexiques");
        printf("\n  [5] Charger un fichier dans un lexique");
        printf("\n  [6] Quitter");
        printf("\n========================================");
        printf("\nVotre choix : ");
        choixMenu = getchar();
        viderBuffer();

        char mot[256];
        t_mot **lexique = NULL;

        switch (choixMenu) {
            case '1':
                lexique = (choisirLexique() == 1) ? &lexique1 : &lexique2;
                printf("\n\n--- Lexique ---\n");
                afficherMots(*lexique);
                break;

            case '2':
                lexique = (choisirLexique() == 1) ? &lexique1 : &lexique2;
                do {
                    printf("\nEntrez le mot à ajouter : ");
                    if (fgets(mot, sizeof(mot), stdin) == NULL) {
                        mot[0] = '\0';
                    } else {
                        mot[strcspn(mot, "\n")] = '\0';
                    }
                    nettoyerMot(mot);
                } while (strlen(mot) == 0);
                *lexique = ajouterMot(*lexique, convertir_minuscule(mot));
                break;

            case '3':
                lexique = (choisirLexique() == 1) ? &lexique1 : &lexique2;
                printf("\nEntrez le mot à retirer : ");
                scanf("%255s", mot);
                viderBuffer();
                *lexique = retirerMot(*lexique, convertir_minuscule(mot));
                break;

            case '4':
                printf("\nFusion des lexiques en cours...\n");
                lexique1 = fusionner(lexique1, lexique2);
                lexique2 = NULL;
                break;

            case '5':
                lexique = (choisirLexique() == 1) ? &lexique1 : &lexique2;
                *lexique = importerFichier(*lexique);
                break;

            case '6':
                printf("\n\n*** FIN DU PROGRAMME ***\n");
                break;

            default:
                printf("\nChoix invalide. Veuillez réessayer.\n");
                break;
        }

    } while (choixMenu != '6');

    libererLexique(lexique1);
    libererLexique(lexique2);

    return 0;
}
