
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <SDL.h>
#include <stdint.h>
#include "stack.h"


#define ERROR(PHRASE, A){fprintf(stderr, "Error - %s \
before %s - word %d\n", PHRASE, CURRENTWRD, p->count);\
exit(A);}
#define ERRORINPUT(PHRASE, A){fprintf(stderr, "Error - \
%s\n", PHRASE); exit(A);}
#define CORRECT_INPUT 2
#define strsame(A,B) (strcmp(A,B) == 0)
/*below use binary operators to shift bits to double or half - half can only be used with even numbers or will loose 1 - this works at present as it is used to find half SIZE which is even*/
#define DOUBLE(A) ((A)<<1)
#define HALF(A) ((A)>>1)
#define INITIALINPUT 100
#define MAXSIZE 8
#define MAXSTRINGIN "%7s"
#define TRUE 1
#define FALSE 0
#define NEXTWRD p->count = p->count + 1
#define CURRENTWRD p->wrds[p->count]
#define LASTWRD p->wrds[p->count -1]
#define PI_APPROX 3.14159265359
#define COMBINED_ANGLES_TRI 180
#define SIZE 750
#define MSECDELAY 50
#define ERROR_SDL(PHRASE){fprintf(stderr, "Error: %s %s\n"\
, PHRASE, SDL_GetError()); SDL_Quit(); exit(1);}
#define WHITE 255, 255, 255, SDL_ALPHA_OPAQUE
#define BLACK 0, 0, 0, SDL_ALPHA_OPAQUE
#define GET_FROM_STACK current = pop(&s); tmp = pop(&s);
#define ALPHABET 26
#define NORTH 270
#define FULLCIRCLE 360
#define ZERO(A) (((A) >= 0.0) && ((A) < 0.0001))
#define NOT_SET_VAL 9999.9999
#define NOT_SET(A) (((A) > 9999.9998) && ((A) < 9999.99999))

enum ErrorCodes{
  ErrInputLen,
  ErrFile,
  ErrInput,
  ErrOpenBrack,
  ErrCloseBrack,
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
  ErrVarNotSet,
  ErrPolBegin,
  ErrPolish,
  ErrDo,
  ErrDoBrack,
  ErrDoLoop,
  ErrFrom,
  ErrTo,
  ErrStack,
  ErrStackEmpt,
  ErrZeroDiv,
  ErrScreenEdge,
  ErrDec
};


struct prog{
  char **wrds;
  double Var[ALPHABET];
  int count;
  int size;
};

struct SDL_Simplewin{
  SDL_bool finished;
  SDL_Window *win;
  SDL_Renderer *renderer;
  int x;
  int y;
  int x1;
  int y1;
  int angle;
};

typedef struct prog Prog;
typedef struct SDL_Simplewin Window;


void checkInput(int argc, FILE *file);
Prog* initialiseProg(int num, Prog *p);
void setPVar(Prog *p);
Prog* readWords(FILE *file, Prog *p);
Prog* increaseWrdSpace(Prog *p);
void insertString(char *str, Prog *p, int i);
void freeProg(Prog *p);
void test(Prog *p);
void testBrackets(Prog *p);
void code(Prog *p, Window *wdw);
void isInstruction(Prog *p, Window *wdw);
uint8_t isVarnum(Prog *p);
uint8_t isVariable(Prog *p);
uint8_t isNegVariable(Prog *p);
void testDo(Prog *p, Window *wdw);
void testDoLoop(Prog *p, Window *wdw);
void checkValues(Prog *p);
void testFrom(Prog *p, int tmp);
void testTo(Prog *p);
double testSet(Prog *p);
double testPolish(Prog *p);
uint8_t isOperator(char c);
void applyFD(Prog *p, Window *wdw);
void find_x_y(Window *wdw, int dist);
void testScreenEdge(Window *wdw);
void applyRT(Prog *p, Window *wdw);
void applyLT(Prog *p, Window *wdw);
void initialiseWindow(Window *wdw);
void runWindow(Window *wdw);
void drawLine(Window *wdw, int x1, int y1, int x2, int y2);
void Neill_SDL_Events(Window *sw);
double findCurrent(Prog *p);
void varSet(Prog *p, int i);
double getNum(Prog *p);
void applyDoLoop(Prog *p, Window *wdw, int i, int n,
     int tmp);
void setWdwVaribles(Window *wdw);
void applyInstr(Prog *p, Window *wdw);
double calculateCurrent(Prog *p, double tmp,
       double current);
void freeWin(Window *wdw);


