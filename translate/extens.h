#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <SDL.h>
#include <stdint.h>

#define ERROR(PHRASE, A){fprintf(stderr, "Error - %s\n",\
 PHRASE); exit(A);}
#define ERROROUT(PHRASE){printf("%s\n", PHRASE);}
#define ERRORINPUT(PHRASE, A){fprintf(stderr, "Error - \
%s\n", PHRASE); exit(A);}
#define ADDERROR(PHRASE) insertError(p, p->count, PHRASE)
#define CORRECT_INPUT 2
#define strsame(A,B) (strcmp(A,B) == 0)
#define DOUBLE(A) ((A)*2)
#define HALF(A) ((A)>>1)
#define INITIALSIZE 100
#define STACKSIZE 500
#define MAX_QUEUE 2000
#define TRUE 1
#define FALSE 0
#define STRSIZE 50
#define NEXTWRD p->count = p->count + 1
#define CURRENTWRD p->wrds[p->count]
#define LASTWRD p->wrds[p->count -1]
#define ADDTOQ InsertQ(p->wrds[p->count-1], &p->q);\
 InsertQ(p->wrds[p->count], &p->q);
#define PI_APPROX 3.14159265359
#define COMBINED_ANGLES_TRI 180
#define SIZE 750
#define MSECDELAY 10
#define ERROR_SDL(PHRASE){fprintf(stderr, "Error: %s %s\n"\
, PHRASE, SDL_GetError()); SDL_Quit(); exit(1);}
#define WHITE 255, 255, 255, SDL_ALPHA_OPAQUE
#define BLACK 0, 0, 0, SDL_ALPHA_OPAQUE
#define GET_FROM_STACK current = pop(&s, p); tmp = pop(&s, p);
#define ALPHABET 26
#define NORTH 270
#define FULLCIRCLE 360
#define ZERO(A) (((A) >= 0.0) && ((A) < 0.0001))
#define NOT_SET_VAL 9999.9999
#define NOT_SET(A) (((A) > 9999.9998) && ((A) < 9999.99999))
#define COMPILEERRORS 6
#define SWAP tmp = str[i]; str[i] = str[i-1]; str[i-1] = tmp;
#define WRITEFILE(A, B) fwrite((A), sizeof(char), (B), fp2);
#define ALLOCATE(A, B, C) if(((A) = (char*)calloc((B), (C)))\
  == NULL){ERROR("Cannot Allocate Space", ErrAlloc);}


enum ErrorCodes{
  ErrUnknown, ErrInitialise, ErrString, ErrAlloc, ErrSys, 
  ErrFile, ErrInput
};

enum Type{
  lower_case, equals, not_known, space, decimal,
  neg, equal, polish, zero, openBrack, closeBrack, brackets,
  instruc, varnum,  variable, doLoop, varset, scrBound
};

typedef enum Type Type;

enum Comment{
  none, warning, recommend, error
};
typedef enum Comment Comment;

struct Tokens{
  char *str;
  int tokens;
  int DoFlag;
};
typedef struct Tokens Tokens;

struct queue{
  char s[MAX_QUEUE][STRSIZE];
  int front;
  int back;
};
typedef struct queue Queue;

struct prog{
  char **wrds;
  int **errors;
  Queue q;
  int Qflag;
  int DoFlag;
  double Var[ALPHABET];
  int count;
  int size;

};
typedef struct prog Prog;

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
typedef struct SDL_Simplewin Window;

struct stackelem{
  double i;
};
typedef struct stackelem Elem;

struct thestack{
  struct stackelem stk[STACKSIZE];
  int top;
};
typedef struct thestack Stack;

char* increaseWrdSpace(Tokens* s, int len);
void getWords(FILE *file, Tokens *s);
char* InitialiseString(char *s, int i);
void checkInput(int argc, FILE *file);
Prog* initialiseProg(int num, Prog *p);
void setPVar(Prog *p);
void insertString(char *str, Prog *p, int i);
void clearProg(Prog *p);
void removeStr(Prog *p, int i);
void freeProg(Prog *p);
void insertError(Prog* p, int i, int err);
Prog* Tokenize(Prog* p, Tokens *s);
int testNumToken(Tokens *s, int i, Prog *p, int flag);
void IsLower(Tokens *s, Prog *p);
int testVar(Tokens *s, int i, Prog* p);
int testF(Tokens *s, int i, Prog *p, char *tmp);
int testLR(Tokens *s, int i, Prog *p, char *tmp);
int testS(Tokens *s, int i, Prog *p, char *tmp);
int testDT(Tokens *s, int i, Prog *p, char *tmp);
int testEquals(Tokens *s, int i, Prog *p, char *tmp);
int testOther(Tokens *s, int i, Prog *p);
int testNegative(Tokens *s, int i, Prog *p, char *tmp);

/*queue functions*/
void InitialiseQueue(Queue *q);
char* RemoveQ(Queue *q);
void InsertQ(char *n, Queue *q);
int EmptyQ(Queue *q);

/*error functions*/
void printErrorsToken(Prog *p, Tokens *s);
void errorStatement(Prog *p, int i);
void adjustProg(Prog *p);
void deleteNull(Prog *p);
void checkspaces(Prog* p);
void errorSyntax(Prog *p, int i);
void errorStatement(Prog *p, int i);
void printErrorSyntax(Prog *p);

/*tokens functions*/
void test(Prog *p);
void testBrackets(Prog *p);
void code(Prog *p, Window *wdw);
void isInstruction(Prog *p, Window *wdw);
void applyInstr(Prog *p, Window *wdw);
void applyFD(Prog *p, Window *wdw);
void testScreenEdge(Window *wdw, Prog *p);
void applyRT(Prog *p, Window *wdw);
void applyLT(Prog *p, Window *wdw);
void find_x_y(Window *wdw, int dist);
uint8_t isVarnum(Prog *p);
uint8_t isVariable(Prog *p);
uint8_t isNegVariable(Prog *p);
void testDo(Prog *p, Window *wdw);
void testDoLoop(Prog *p, Window *wdw);
void testFrom(Prog *p, int tmp);
void testTo(Prog *p);
void checkValues(Prog *p);
void applyDoLoop(Prog *p, Window *wdw, int i, int n,
     int tmp);
double testSet(Prog *p);
double findCurrent(Prog *p);
void varSet(Prog *p, int i);
double getNum(Prog *p);
double testPolish(Prog *p);
double calculateCurrent(Prog *p, double tmp, 
       double current);
uint8_t isOperator(char c);

/*SDL functions*/
void setWdwVaribles(Window *wdw);
void initialiseWindow(Window *wdw);
void drawLine(Window *wdw, int x1, int y1, int x2, int y2);
void freeWin(Window *wdw);
void Neill_SDL_Events(Window *sw);

/*stack functions*/
void initialiseStack(Stack *s);
void push(Stack * s, double n, Prog* p);
double pop(Stack *s, Prog *p);
uint8_t emptyStack(Stack *s);

/*file write functions*/
int checkErrors(Prog* p);
FILE* MakeCFile(FILE* fp2, Prog *p);
FILE* testVariables(Prog *p, FILE *fp2);
FILE* convertQ(Prog *p, FILE *fp2);
FILE* writeDO(Prog *p, FILE* fp2);
FILE* writeSET(Prog *p, FILE *fp2);
FILE* writeFD(Prog *p, FILE *fp2);
FILE* writeRT(Prog *p, FILE* fp2);
FILE* writeLT(Prog *p, FILE* fp2);
int oper(char* str);
FILE* ReversePolish(Prog *p, FILE *fp2);
int isvari(char* str);
int findlen(Prog *p);
void swapPol(char **str, int i);
void move(Window *wdw, int a);
void turnLeft(Window *wdw, int a);
void turnRight(Window *wdw, int a);
FILE* addend(FILE *fp2);
FILE* addtopage(FILE *fp2);
void Compile(char c);
void writePolish(char **str, int n, FILE *fp2);




