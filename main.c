#include "tp3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NB_LEXIQUES 5  

int main() {
    char choixMenu = '0';
    t_mot **lexiques = calloc(NB_LEXIQUES, sizeof(t_mot*)); 

    if (lexiques == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return 1;
    }

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
        int index;

        switch (choixMenu) {
            case '1':
                index = choisirLexique(NB_LEXIQUES);
                printf("\n--- Lexique %d : affichage en cours ---\n", index + 1);
                afficherMots(lexiques[index]);
                printf("Affichage terminé.\n");
                break;

            case '2':
                index = choisirLexique(NB_LEXIQUES);
                printf("\nEntrez le mot à ajouter : ");
                if (fgets(mot, sizeof(mot), stdin) != NULL) {
                    mot[strcspn(mot, "\n")] = '\0';
                    lexiques[index] = ajouterMot(lexiques[index], mot);
                    printf("Mot '%s' ajouté au lexique %d.\n", mot, index + 1);
                }
                break;

            case '3':
                index = choisirLexique(NB_LEXIQUES);
                printf("\nEntrez le mot à retirer : ");
                scanf("%255s", mot);
                viderBuffer();
                lexiques[index] = retirerMot(lexiques[index], mot);
                printf("Mot '%s' retiré du lexique %d.\n", mot, index + 1);
                break;

            case '4': {
                int idx1 = choisirLexique(NB_LEXIQUES);
                int idx2 = choisirLexique(NB_LEXIQUES);
                if (idx1 != idx2) {
                    printf("\nFusion des lexiques %d et %d...\n", idx1 + 1, idx2 + 1);
                    lexiques[idx1] = fusionner(lexiques[idx1], lexiques[idx2]);
                    lexiques[idx2] = NULL;
                    printf("Fusion terminée : lexique %d contient maintenant la fusion.\n", idx1 + 1);
                } else {
                    printf("\nVous avez choisi le même lexique pour la fusion. Aucune action effectuée.\n");
                }
                break;
            }

            case '5':
                index = choisirLexique(NB_LEXIQUES);
                printf("\nImportation d'un fichier dans le lexique %d...\n", index + 1);
                lexiques[index] = importerFichier(lexiques[index]);
                printf("Importation terminée pour le lexique %d.\n", index + 1);
                break;

            case '6':
                printf("\n*** FIN DU PROGRAMME ***\n");
                break;

            default:
                printf("\nChoix invalide. Veuillez réessayer.\n");
                break;
        }

    } while (choixMenu != '6');

    libererTousLexiques(lexiques, NB_LEXIQUES);
    printf("Toutes les ressources ont été libérées.\n");
    return 0;
}
