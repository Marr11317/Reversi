#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {
      true  = 1,
      false = 0
}bool;
char* coup;
int tour = 1;

// 36 * 13 + 1= 469
// plus 1 pour le \n final
char jeu[469] = "               --------------------\n"
                "             10|                   \n"
                "             9 |                   \n"
                "             8 |                   \n"
                "             7 |                   \n"
                "             6 |        O X        \n"
                "             5 |        X O        \n"
                "             4 |                   \n"
                "             3 |                   \n"
                "             2 |                   \n"
                "             1 |                   \n"
                "               --------------------\n"
                "                A B C D E F G H I J\n\n";

int antiModulo(const int x, int moduloCombien) {
      int a = x;
      int i = 0;
      while (a >= moduloCombien) {
            a -= moduloCombien;
            i++;
      }
      return i;
}
int posDansJeu(const int x, const int y) {
      return ((11 - y) * 36 + ((2 * x) + 15) - 1);
}

void posDansTab(const int pos, int *x, int *y) {
      int valx = (pos % 36) / 2 - 1;
      int valy = 11 - (antiModulo(pos, 36));
      *x = valx;
      *y = valy;
}

void rotaterJeu(){
      char copieJeu[468] = {0};
      int i = 0, j = 0;
      for (i = 0; i < 468; i++)
            copieJeu[i] = jeu[i];

      for (i = 1; i <= 10; i++) {
            for (j = 1; j <= 10; j++) {
                  jeu[posDansJeu(i, j)] = copieJeu[posDansJeu(-j + 11, i)];
            }
      }
}

void rotaterPoint(int* x, int* y) {
      int cpx = *x;

      *x = *y;
      *y = -cpx + 11;
}

int tournerJetons(int x, int y) {
      char couleur = (tour % 2 ? 'X' : 'O');
      char couleurEnnemie = (tour % 2 ? 'O' : 'X');

      int nbTournes = 0;
      char a = couleurEnnemie;
      int r = 0, i = 1;
      for (r = 0; r < 4; r++) {
            a = couleurEnnemie;
            rotaterPoint(&x, &y);
            rotaterJeu();
            // Vers le haut.
            for (i = 1; a == couleurEnnemie; i++) {
                  a = jeu[posDansJeu(x, y + i)];
                  if (a == couleur) {
                        i--;
                        a = jeu[posDansJeu(x, y + i)];
                        while (a == couleurEnnemie) {
                              jeu[posDansJeu(x, y + i)] = couleur;
                              nbTournes++;
                              i--;
                              a = jeu[posDansJeu(x, y + i)];
                        }
                  }
            }
            a = couleurEnnemie;
            // Vers la diagonale haut-droit
            for (i = 1; a == couleurEnnemie; i++) {
                  a = jeu[posDansJeu(x + i, y + i)];
                  if (a == couleur) {
                        i--;
                        a = jeu[posDansJeu(x + i, y + i)];
                        while (a == couleurEnnemie) {
                              jeu[posDansJeu(x + i, y + i)] = couleur;
                              nbTournes++;
                              i--;
                              a = jeu[posDansJeu(x + i, y + i)];
                        }
                  }
            }
      }
      return nbTournes;
}

bool faireCoup() {
      printf("             Votre coup: ");
      scanf("%s", coup);

      char coupX = 0;
      int coupY = 0;

      sscanf(coup, "%c%d", &coupX, &coupY);

      int posX = (int) *strupr(&coupX) - 'A' + 1;
      if (posX <= 0 || posX >= 11) {
            printf("             Vous devez choisir comme position en x une lettre entre A et J.\n");
            return false;
      }
      if (coupY <= 0 || coupY >= 11) {
            printf("             Vous devez choisir comme position en y un nombre entre 1 et 10.\n");
            return false;
      }

      if (jeu[posDansJeu(posX, coupY)] == 'X' && jeu[posDansJeu(posX, coupY)] == 'O') {
            printf("             La case est deja occupee\n");
            return false;
      }

      jeu[posDansJeu(posX, coupY)] = (tour % 2 ? 'X' : 'O');

      // i.e. si aucun jetons n'est tourne, donc le coup est invalide:
      if (!tournerJetons(posX, coupY)) {
            printf("             Ce coup est invalide: il ne tourne aucun jetons.\n");
            jeu[posDansJeu(posX, coupY)] = '*'; // enlever le jeton.
            return false;
      }

      return true;
}

void compter(int *x, int *O) {
      int compteX = 0;
      int compteO = 0;
      int i = 0;
      for (i = 1; i <= 10; i++) {
            int j = 0;
            for (j = 1; j <= 11; j++) {
                  char a = jeu[posDansJeu(i, j)];
                  if (a == 'X')
                        compteX++;
                  else if (a == 'O')
                        compteO++;
            }
      }
      *x = compteX;
      *O = compteO;
}

void printSmiles() {
      printf("             ");
      int i = 0;
      for (i = 0; i < 100; i++)
            printf("\0x07");

      printf("\n");
}

int main()
{
      coup = malloc(sizeof(char) * 10);
      int i = 1, j = 1;
      for (i = 0; i < 10; i++)
            coup[i] = 0;

      char copieJeu[469] = {0};
      i = 0, j = 0;
      for (i = 0; i < 469; i++)
            copieJeu[i] = jeu[i];

      bool quitter = false;
      while (!quitter) {
            for (i = 1; i <= 10; i++) {
                  for (j = 1; j <= 10; j++) {
                        if (jeu[posDansJeu(i, j)] == ' ')
                              jeu[posDansJeu(i, j)] = '*';
                  }
            }

            printf("\n"
                   "       *****************************************************************\n"
                   "       *                                                               *\n"
                   "       *  Bonjour et bienvenue a cette premiere edition de Reversi!!!  *\n"
                   "       *  Voici le jeu de depart. Jouez en entrant les coordonnees.    *\n"
                   "       *                                                               *\n"
                   "       *****************************************************************\n\n");
            printf("              -- Debut\n");
            printf(jeu);

            int nbX;
            int nbO;
            for (tour = 1; tour <= 100; tour++) {
                  if (tour % 2)
                        printf("\n             Aux X de jouer!!\n");
                  else
                        printf("\n             Aux O de jouer!!\n");

                  bool valide = false;
                  do {
                        valide = faireCoup();
                  } while (!valide);

                  printf("\n              -- Tour %d: %s\n", tour, coup);
                  printf(jeu);

                  compter (&nbX, &nbO);
                  printf ("             %d chez les X, et %d chez les O!\n", nbX, nbO);
                  if (tour != 100) {
                        if (nbX > nbO)
                              printf("             Une dominance des X sur le terrain, mesdames et messieurs.\n"
                                     "             Oui oui, une dominance des X!\n");
                        else if (nbO > nbX)
                              printf("             Hohoho, le top pour les O.\n"
                                     "             On les applaudit bien fort!\n");
                        else
                              printf("             Un match tendu entre deux adversaires ici: nous avons egalite.\n"
                                     "             E-ga-li-te!\n");
                  }
            }
            if (nbX > nbO)
                  printf("             Une foule en delire pour ce match incroyable.\n"
                         "             Bravo, bravo! applaudissez les X!\n");
            else if (nbO > nbX)
                  printf("             Le haut du podium est pour les O...\n"
                         "             Euhhh.... \n"
                         "             Bon, d'accord... elle etait pas drole...\n");
            else
                  printf("             WAHHHHHHHHH... E-GA-LI-TE!!!\n");

            printf("             Voulez-vous rejouer (o ou n)\n");
            char a = getchar();
            if (a == 'n')
                  quitter = true;
            for (i = 0; i < 469; i++)
                  jeu[i] = copieJeu[i];

      }

      return 0;
}


