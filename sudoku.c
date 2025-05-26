#include <stdio.h>
#include <stdlib.h>


/////////////////////////////////////////////////////////////////
//////////////// Déclaration constantes et types ////////////////
/////////////////////////////////////////////////////////////////


#define MAX1 30                     // Constante Taille maximum tableau 2D
#define MAX2 9                      // Constante Taille maximum tableau 2D

typedef int tab1D[MAX1];			// Définition type tab1D (tableau 1D)
typedef int tab2D[MAX2][MAX2];		// Définition type tab2D (tableau 2D)


////////////////////////////////////////////////////////////////
//////////////// Declaration Variables Globales ////////////////
////////////////////////////////////////////////////////////////


tab1D t;                            // Délaration  tableau 1D d'entiers
int dim;                            // Dimension (taille) tableau 1D O

tab2D m;                            // Déclaration matrice tableau 2D d'entiers
int n;                              // Ordre matrice carré 2D
int choixUtil;                      // choix utilisateur
int solutionTrouvee = 0;            


//////////////////////////////////////////////////////////////
//////////////// Declaration variables projet ////////////////
//////////////////////////////////////////////////////////////


tab2D sudoku= {
    {0,0,4,5,0,0,1,3,6},
    {0,0,0,3,0,0,8,2,0},
    {3,8,0,0,6,0,0,0,0},
    {0,0,5,0,0,0,0,1,8},
    {0,0,0,0,0,0,0,0,0},
    {9,2,0,0,0,0,7,0,0},
    {0,0,0,0,3,0,0,6,5},
    {0,7,9,0,0,4,0,0,0},
    {5,1,3,0,0,8,2,0,0}
};                                  //Declaration et Initialisation Grille initiale sudoku

  
tab2D carre;                        //Bloc carre (3x3) extrait de la grille

int i, j;                           //indices case(i,j) pour placer un chiffre 
int c;                              //chiffre c à placer


//////////////////////////////////////////////////////////////
////////////////// Procédure Saisir Matrice //////////////////
//////////////////////////////////////////////////////////////


void saisirMat(tab2D mat, int n) {
    int i, j;
    
    printf("\n\n --> Saisie Matrice Carre\n\n");
    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            printf("Entrer mat[%d][%d] : ", i, j);
            scanf("%d", &mat[i][j]);
        }
    }
}


//////////////////////////////////////////////////////////////
///////////////// Procédure Afficher Matrice /////////////////
//////////////////////////////////////////////////////////////


void afficherMat(tab2D mat, int n) {
    int i, j;

    printf("\n\n --> Affichage Matrice Carre\n\n");
    for (i = 0; i < n; i++){
        if (i % 3 == 0) {
            printf(" -------------------------\n");
        }
        for (j = 0; j < n; j++){
            if (j % 3 == 0) {
                printf(" |%2d", mat[i][j]);
            } else {
                printf("%2d", mat[i][j]);

            }
        }
        printf(" |\n");
    }
    printf(" -------------------------\n");
}


////////////////////////////////////////////////////////////////////
///////////////// Fonction Chiffre Possible (I, J) /////////////////
////////////////////////////////////////////////////////////////////


int chiffrePossibleLigneIColonneJ(tab2D sudoku, int n, int i, int j, int c) {
    int k, x, y;

    for (k = 0; k < n; k++) {
        if (sudoku[i][k] == c || sudoku[k][j] == c) {
            return 0;
        }
    }

    return 1;
}


////////////////////////////////////////////////////////////////
///////////////// Fonction Chercher Case Libre /////////////////
////////////////////////////////////////////////////////////////


void chercheCaseLibre(tab2D sudoku, int n, int *pi, int *pj) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (sudoku[i][j] == 0) {
                *pi = i;
                *pj = j;
                return;
            }
        }
    }

    *pi = -1;
    *pj = -1;
}


/////////////////////////////////////////////////////////////////
///////////////// Fonction Complètement Remplie /////////////////
/////////////////////////////////////////////////////////////////


int estCompletementRemplie(tab2D sudoku, int n) {
    int i, j;

    chercheCaseLibre(sudoku, n, &i, &j);
    return (i == -1 && j == -1);
}


///////////////////////////////////////////////////////////
///////////////// Procédure Extraire Bloc /////////////////
///////////////////////////////////////////////////////////


void extraireBloc(tab2D sudoku, int x, int y, tab2D carre) {
    int mx = x / 3;
    int my = y / 3;
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            carre[i][j] = sudoku[mx*3+i][my*3+j];
        }
    }
}


//////////////////////////////////////////////////////////////
///////////////// Fonction Fréquence Chiffre /////////////////
//////////////////////////////////////////////////////////////


int frequenceChiffre(tab2D carre, int n, int c) {
    int count = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (carre[i][j] == c) {
                count++;
            }
        }
    }
    return count;
}  


////////////////////////////////////////////////////////////////
///////////////// Fonction Aucun Doublon Carré /////////////////
////////////////////////////////////////////////////////////////


int aucunDoublonCarre(tab2D carre, int n) {
    int i, j;

    int frequence[10] = {0};
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            int c = carre[i][j];
            if (c != 0) {
                if (frequence[c] > 0) {
                    return 0;
                }
                frequence[c]++;
            }
        }
    }
    return 1;
}


////////////////////////////////////////////////////////////////
///////////////// Procédure Chercher Solutions /////////////////
////////////////////////////////////////////////////////////////


void chercheSolutions(tab2D sudoku, int n) {
    int i, j;

    chercheCaseLibre(sudoku, n, &i, &j);

    if (estCompletementRemplie(sudoku, n)) {
        afficherMat(sudoku, n);
        printf("\n");
        solutionTrouvee = 1;
        return;
    }

    for (int c = 1; c <= 9; c++) {
        if (chiffrePossibleLigneIColonneJ(sudoku, n, i, j, c)) {
            sudoku[i][j] = c;

            extraireBloc(sudoku, i, j, carre);
            if (aucunDoublonCarre(carre, 3)) {
                chercheSolutions(sudoku, n);
            }

            sudoku[i][j] = 0;
        }
    }
}


///////////////////////////////////////////////////////
///////////////// Fonction Lire Choix /////////////////
///////////////////////////////////////////////////////


int  lireChoix(){
    int choix;

    printf(" \n\n\n******************* Menu Grilles Sudoku ************************ \n\n\n");
    printf("\t 0: Quitter le programme\n");
    printf("\t 1: Saisir Matrice Carre d'ordre n  \n");
    printf("\t 2: Afficher Matrice Carre d'ordre n \n");
    printf("\t 3: Chiffre Possible a la Ligne I et a la Colonne J  \n");
    printf("\t 4: Cherche Premiere Case Libre  \n");
    printf("\t 5: Grille completement remplie  \n");
    printf("\t 6: Test Extraire Bloc (Carre 3x3), Frequence Chiffre, Aucun Doublon \n");
    printf("\t 7: Chercher les solutions de la grille \n");
    printf(" \n\n****************************************************************\n\n\n ");

    printf("\n Tapez le numero de votre choix [0,7] -> ");
    scanf("%d",&choix);

    return choix;
}


////////////////////////////////////////////////////////////
///////////////// Procédure Executer Choix /////////////////
////////////////////////////////////////////////////////////


void execChoix(int choix){
    switch (choix) {
        case 0: {                   // Quitter 
            printf("\n\n Fin du programme Manipulation des Tableaux  , Bye, Bye \n\n");
            exit(1);
        }     
        
        case 1: {                   //Saisie Tableau 1D de dimension dim 
            printf("\n\n --> Saisie Matrice 2D d'ordre n \n");
            printf("\n Donner ordre matrice : ");
            scanf("%d",&n);

            saisirMat(m,n);
            break;
        }
        
        case 2: {                   // Affichage Matrice d'ordre n
            printf("\n\n --> Affichage matrice carre d'ordre n \n");
            afficherMat(m,n);
            break;
        }
                    
        case 3: {                   // Chiffre possible 
            int c;
            printf( "\n\n---> Test fonction chiffre Possible pour la Ligne I et la Colonne J \n ");
            printf("Chiffre : ");
            scanf("%d", &c);
            printf("%d", chiffrePossibleLigneIColonneJ(sudoku, 9, 0, 0, c));
            break;
        }
                
        case 4: {                   // Cherche premiere case libre
            printf("\n\n ---> Test fonction cherche case libre ");
            int i, j;
            chercheCaseLibre(sudoku, MAX2, &i, &j);
            printf("%d, %d", i, j);
            break;
        }
                
        case 5: {                    // Grille completement remplie
            printf("\n\n ---> Test Grille completement remplie \n"); 
            break;
        }
                
        case 6: {                   // Extraire Bloc, Frequence Chiffre, Aucun Doublon
            int x, y;
            printf("\n\n ---> Test Extraire Bloc, FrequenceChiffre, Aucun Doublon \n");
            printf("x : ");
            scanf("%d", &x);
            printf("y : ");
            scanf("%d", &y);
            
            extraireBloc(sudoku, x, y, carre);
            afficherMat(carre, 3);
            break;
        }
        
        case 7: {                   // Recherche solutions
            printf("\n\n ---> Test fonction chercheSolutions \n");  
            chercheSolutions(sudoku, 9);
            break;
        }        
        
        default: {
            printf("BUG! valeur %d incorrecte dans execChoix()\n",choix);
            exit(1);
        }  
    }              
}


//////////////////////////////////////////////////////////////
///////////////// Fonction Principale (main) /////////////////
//////////////////////////////////////////////////////////////


int main() {
    printf("\n\n =================  Manipulation Grilles Sudoku avec Menu ============ \n\n\n");  
    
    do{
        choixUtil= lireChoix();
        execChoix(choixUtil); 
        printf("\n");    
    } while (choixUtil != 0);

    system("pause"); 
    return 0;
}
