#include "extens.h"

void initialiseStack(Stack *s)
{
  s->top = 0;
}

void push(Stack * s, double n, Prog *p)
{

  if(s->top < STACKSIZE){
  s->stk[s->top].i = n;
  s->top = s->top + 1;
  }
  else{
    ADDERROR(polish);
    return;
  }
}

double pop(Stack *s, Prog *p)
{
  if(s->top > 0){
  s->top = s->top - 1;
  }
  else{
    ADDERROR(polish);
  }

  return s->stk[s->top].i;
}

uint8_t emptyStack(Stack* s)
{
  return(s->top == 0) ? TRUE:FALSE;
}
