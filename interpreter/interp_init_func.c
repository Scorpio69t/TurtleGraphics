#include "interp.h"

void checkInput(int argc, FILE *file)
{

  if(argc != CORRECT_INPUT){
    ERRORINPUT("Need file to check\n", ErrInputLen);
  }
  
  if(file == NULL){
    ERRORINPUT("Cannot open file\n", ErrFile);
  }

}

Prog* initialiseProg(int num, Prog *p)
{

  if((p = (Prog*)calloc(1, sizeof(Prog)))
    == NULL){ERROR("Cannot allocate Program", ErrAllocTab);
  }

  if((p->wrds = (char**)calloc((size_t)num,sizeof(char*)))
    == NULL){ERROR("Cannot allocate array", ErrAllocArr);
  }

  p->size = num;
  setPVar(p);

  return p;

}


/*sets all variables to number to test against to see if
  they have been set by the user. The user cannot set to 
  this value as the maximum input is 8 characters*/
void setPVar(Prog *p)
{
  int i;

  for(i = 0; i < ALPHABET; i++){
    p->Var[i] = NOT_SET_VAL;
  }


}

/*MAXSIZE set to 8 to allow for large numbers with two 
  decimal places. All other instructions fit within this 
  size MAXSTRINGIN set to 7 to allow for NULL chars - 
  strings longer will fil prog but will also go far off
  screen*/
Prog* readWords(FILE *file, Prog *p)
{

  char str[MAXSIZE] = {'\0'};
 
  p->count = 0;

  while(fscanf(file, MAXSTRINGIN, str) == 1){
    if(p->count == p->size-1){
      p = increaseWrdSpace(p);
    }
    insertString(str, p, p->count);
  }

  return p;

}

/*function to double size of array - this is in place to 
allow for programs of any size to be parsed*/
Prog* increaseWrdSpace(Prog *p)
{

  int i;
  Prog *tmp = NULL;

  tmp = initialiseProg(DOUBLE(p->size), tmp);

  for(i = 0; i < p->count; i++){
    insertString(p->wrds[i], tmp, i);
  }

  tmp->count = p->count;
  freeProg(p);
  
  return tmp;

}

void insertString(char *str, Prog *p, int i)
{

  size_t len = strlen(str);

  if((p->wrds[i] = (char*)calloc(1, len+1)) == NULL){
    ERROR("Cannot allocate space for string", ErrAllocStr);
  }
 
  strncpy(p->wrds[i], str, len);
  p->count++;

}

void freeProg(Prog *p)
{

  int i;

  for(i = 0; i < p->size; i++){
    free(p->wrds[i]);
  }

  free(p->wrds);
  free(p);

}
