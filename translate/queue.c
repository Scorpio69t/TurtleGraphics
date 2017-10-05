#include "extens.h"

void InitialiseQueue(Queue *q)
{
  q->front = 0;
  q->back = 0;
}

char* RemoveQ(Queue *q)
{
  char *n = NULL;
  int len = strlen(q->s[q->front]);
  
  ALLOCATE(n, 1, len);
  memcpy(n, q->s[q->front], len);
  q->front = (q->front + 1) % MAX_QUEUE;

  return n;
}

void InsertQ(char *n, Queue *q)
{
  int len = strlen(n);

  memcpy(q->s[q->back], n, len);
  q->back = (q->back +1) % MAX_QUEUE;
}

int EmptyQ(Queue *q)
{
  return(q->front == q->back) ? 1:0; 
}
