/*simple stack functions*/
#include "stack.h"

void initialiseStack(Stack *s)
{
  s->top = 0;
}

void push(Stack * s, double n)
{

  if(s->top < STACKSIZE){
  s->stk[s->top].i = n;
  s->top = s->top + 1;
  }
  else{
    fprintf(stderr, "No room in Stack - Polish expression too long\n");
    exit(1);
  }
}

double pop(Stack *s)
{
  if(s->top > 0){
  s->top = s->top - 1;
  }
  else{
    fprintf(stderr, "Stack Empty - Missing Variables in Set, or polish expresssion wrong\n");
    exit(1);
  }

  return s->stk[s->top].i;
}

uint8_t emptyStack(Stack* s)
{
  return(s->top == 0) ? TRUE:FALSE;
}
