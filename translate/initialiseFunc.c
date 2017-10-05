#include "extens.h"

char* InitialiseString(char *s, int i)
{

  ALLOCATE(s, 1, i);

  return s;

}

void checkInput(int argc, FILE *file)
{

  if(argc != CORRECT_INPUT){
    ERROR("Need file to check\n", ErrInitialise);
  }
  
  if(file == NULL){
    ERROR("Cannot open file\n", ErrInitialise);
  }

}

Prog* initialiseProg(int num, Prog *p)
{

  if((p = (Prog*)calloc(1, sizeof(Prog)))
    == NULL){ERROR("Cannot allocate Program", ErrInitialise);
  }

  if((p->wrds = (char**)calloc((size_t)num,sizeof(char*)))
    == NULL){ERROR("Cannot allocate array", ErrInitialise);
  }

  if((p->errors = (int**)calloc(num, sizeof(int*))) == NULL){
    ERROR("Cannot Allocate Errors", ErrInitialise);
  }

  InitialiseQueue(&p->q);
  p->size = num;
  setPVar(p);
  p->count = 0;
  p->Qflag = 0;
  p->DoFlag = 0;

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

void insertString(char *str, Prog *p, int i)
{

  size_t len = strlen(str);

  ALLOCATE(p->wrds[i], 1, len+1);
 
  strncpy(p->wrds[i], str, len);
  p->count++;
}

void removeStr(Prog *p, int i)
{
  
  free(p->wrds[i]);
  p->wrds[i] = NULL;

}

void insertError(Prog* p, int i, int err)
{
  
  if((p->errors[i] = (int*)calloc(1, sizeof(int))) == NULL){
    ERROR("Cannot Allocate Space for Error", ErrString);
  }
  *p->errors[i] = err;

}

void freeProg(Prog *p)
{

  int i;

  for(i = 0; i < p->size; i++){
    if(p->wrds[i] != NULL){
      free(p->wrds[i]);
    }
    if(p->errors[i] != NULL){
      free(p->errors[i]);
    } 
  }

  free(p->wrds);
  free(p->errors);
  free(p);

}


void adjustProg(Prog *p)
{
  int i = 0;

  checkspaces(p);
  clearProg(p);
  deleteNull(p);

  while(p->wrds[i] != NULL){
    i++;
  }
  p->count = i;

}

void deleteNull(Prog *p)
{
  int i, j = 0;

  if(p->count-1 < 3){
    return;
  }
  for(i = 0; i < p->count-1; i++){
    if(p->wrds[i] == NULL){
      j = i;
      while((p->wrds[j] == NULL) && (j < p->count)){
        j++;
      }
      if(p->wrds[j] != NULL){
        insertString(p->wrds[j], p, i);
        removeStr(p, j);
        p->count--;
      }
      j = 0;
    }
  }   
  p->count--;
}

void clearProg(Prog *p)
{
  int i;

  for(i = 0; i < p->count; i++){
    if(strsame(" ", p->wrds[i]) || (strsame("\r", 
    p->wrds[i])) || (strsame("\n", p->wrds[i]))){
      removeStr(p, i);
    }
  }
}
