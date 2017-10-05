#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STACKSIZE 500
#define TRUE 1
#define FALSE 0

struct stackelem{
  double i;
};

struct thestack{
  struct stackelem stk[STACKSIZE];
  int top;
};

typedef struct stackelem Elem;
typedef struct thestack Stack;

void initialiseStack(Stack *s);
void push(Stack * s, double n, Prog* p);
double pop(Stack *s, Prog *p);
uint8_t emptyStack(Stack *s);

