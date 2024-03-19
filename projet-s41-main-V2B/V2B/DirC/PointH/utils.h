#include "instructions.h"

// La fonction qui exécute l'instruction
void execute_instruction(Instruction OP);


// Affichage d'une instruction
void affInst(Instruction TPI) ;

//  Lecture/ecriture dans un pipe
int lirePipe(int fd);
void ecrirePipe(int fd, int ent) ;

