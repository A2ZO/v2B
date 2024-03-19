#include "instructions.h"

// Pour initialiser le tableau d'instruction
void chargerFichierProg(int fprog, int nbInst, Instruction TPI[]) ;

// Lecture du nombre d'instruction 
int lireNbrInst(int fd) ;

//  Lecture d'une instruction dans un fichier
void lireInst(int fd, Instruction *TPI);