#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>

#define ERROR(PHRASE, A){fprintf(stderr, "Error - %s \
before  %s - word %d\n", PHRASE, CURRENTWRD,\
p->count+1); exit(A);}
#define ERRORINPUT(PHRASE, A){fprintf(stderr, "Error -\
%s\n", PHRASE); exit(A);}
#define CORRECT_INPUT 2
#define strsame(A,B) (strcmp(A,B) == 0)
#define DOUBLE(A) ((A)<<1)
#define INITIALINPUT 100
#define MAXSIZE 10
#define MAXSTRINGIN "%9s"
#define TRUE 1
#define FALSE 0
#define NEXTWRD p->count = p->count + 1
#define CURRENTWRD p->wrds[p->count]
#define LASTWRD p->wrds[p->count-1]
#define DOLENGTH 4

enum ErrorCodes{
  ErrInputLen,
  ErrFile,
  ErrInput,
  ErrOpenBrack,
  ErrAllocStr,
  ErrAllocTab,
  ErrAllocArr,
  ErrInvalidProg,
  ErrInstrucList,
  ErrVarnum,
  ErrInstruc,
  ErrMatchBrk,
  ErrVar,
  ErrVariable,
  ErrMissPol,
  ErrPolish,
  ErrDo,
  ErrDoLoop,
  ErrSet
};

struct prog{
  char **wrds;
  int count;
  int size;
};
typedef struct prog Prog;

void checkInput(int argc, FILE *file);
Prog* initialiseProg(int num, Prog *p);
Prog* readWords(FILE *file, Prog *p);
Prog* increaseWrdSpace(Prog *p);
void insertString(char *str, Prog *p, int i);
void freeProg(Prog *p);
void test(Prog *p);
void testBrackets(Prog* p);
void code(Prog *p);
void isInstruction(Prog *p);
void testFDLTRT(Prog *p);
uint8_t isVarnum(Prog *p);
uint8_t isVariable(Prog *p);
uint8_t isNegVariable(Prog *p);
void testDo(Prog *p);
void testDoLoop(Prog *p);
uint8_t testFrom(Prog *p, int cnt);
uint8_t testTo(Prog *p, int cnt);
void testSet(Prog *p);
void testPolish(Prog *p);
uint8_t isOperator(char c);

