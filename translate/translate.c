/*Includes functions to translate the LOGO file to 
  C code and output this code.
  Magics nums left in this code where writing to file
  takes place - this is because each number is specific
  to the amount of chars in the constant strings being 
  written - some strings make this code messy but necessary
  to ensure right spacing when printed out.
  free(tmp) is used throughout this when string is removed from
  the Queue as space is calloc'ed for each string removed*/

#include "extens.h"

int checkErrors(Prog* p)
{
  int i;

  for(i = 0; i <p->count; i++){
    if(p->errors[i] != NULL){
      if(*p->errors[i] > COMPILEERRORS){
        fprintf(stderr,"Unable to compile this program\
 - too many serious errors\n");
        return FALSE;
      }
    }
  }
  return TRUE;
}

/*sends correct output to command line if called*/
void Compile(char c)
{

  char *str = {"gcc SDL_func.c syntax.c errors.c queue.c \
translate.c tokenize.c initialiseFunc.c stack.c CProg.c -o\
 cProg -I/usr/include/SDL2 -lSDL2 -L/usr/local/lib -lSDL2\
 -lm"};

  if(c != 'Y'){
    return;
  }
  
  if(system(str) == -1){
    ERROR("Failed to call system", ErrSys);
  }

}

/*opens a c file and places constants in correct place
  check queue for variables so that these will be at top
  of file*/
FILE* MakeCFile(FILE* fp2, Prog *p)
{

  char *init = {"#include \"extens.h\"\n\nint\
 main(void)\n{\n  Window wdw;\n"};
  char *end = {"\n\n  return 0;\n}\n"};
  int len = strlen(init);

  if((fp2 = fopen("CProg.c", "w")) == NULL){
    ERROR("Unable to Create File", ErrFile);
  }
  
  printf("If you have very large Do loops in your program\
some parts of it may be missing in the C file\n");

  /*calls all functions to put instructions on file*/
  WRITEFILE(init, len);
  fp2 = testVariables(p, fp2);
  fp2 = addtopage(fp2);
  fp2 = convertQ(p, fp2);
  len = strlen(end);
  fp2 = addend(fp2);
  WRITEFILE(end, len);

  return fp2;
}

FILE* addend(FILE *fp2)
{
  char *tmp = {"\n SDL_Delay(1000);\n  atexit(SDL_Quit);\n"};
  int len = strlen(tmp);

  WRITEFILE(tmp, len);

  return fp2;

}

FILE* addtopage(FILE *fp2)
{
  char *tmp = {"  initialiseWindow(&wdw);\n\
  setWdwVaribles(&wdw);\n\n  "};
  int len = strlen(tmp);

  WRITEFILE(tmp, len);

  return fp2;

}

FILE* convertQ(Prog *p, FILE *fp2)
{

  char *tmp = NULL;
  int bracket = p->DoFlag;

  while(!EmptyQ(&p->q)){ 
    tmp = RemoveQ(&p->q);
    if((strsame(tmp, "}")) && (bracket != 0)){
      WRITEFILE("  }\n", 4);
      bracket--;
    }
    if((strsame(tmp, "DO"))&&(p->DoFlag !=0)){
      fp2 =  writeDO(p, fp2);
      p->DoFlag--;
    }
    if(strsame(tmp, "SET")){
      fp2 = writeSET(p, fp2);
    }
    if(strsame(tmp, "FD")){
      fp2 = writeFD(p, fp2);
    }
    if(strsame(tmp, "RT")){
      fp2 = writeRT(p, fp2);
    }
    if(strsame(tmp, "LT")){
      fp2 = writeLT(p, fp2);
    }
    free(tmp);
  }

  return fp2;
}

/*following functions take strings from file and place 
in correct style in c file. Free needs to be called as
calloc was used for strings in the queue*/
FILE* writeLT(Prog *p, FILE* fp2)
{
  char *tmp = NULL;
  int len = 0;

  WRITEFILE("  turnLeft(&wdw,", 16);
  tmp = RemoveQ(&p->q);
  len = strlen(tmp);
  WRITEFILE(tmp, len);
  WRITEFILE(");\n", 3);
  free(tmp);

  return fp2;

}

FILE* writeRT(Prog *p, FILE* fp2)
{
  char *tmp = NULL;
  int len = 0;

  WRITEFILE("  turnRight(&wdw,",17);
  tmp = RemoveQ(&p->q);
  len = strlen(tmp);
  WRITEFILE(tmp, len);
  WRITEFILE(");\n", 3);
  free(tmp);

  return fp2;

}

FILE* writeFD(Prog *p, FILE *fp2)
{
  char *tmp = NULL;
  int len = 0;

  WRITEFILE("  move(&wdw,", 12);
  tmp = RemoveQ(&p->q);
  len = strlen(tmp);
  WRITEFILE(tmp, len);
  WRITEFILE(");\n", 3);
  free(tmp);

  return fp2;

}

/*in do loop calloc needed to hold the variable so that this
  can be used multiple times in the for loop. Function is 
  long but it made no sense to split this as it is writing 
  the most complex string to the file*/
FILE* writeDO(Prog *p, FILE* fp2)
{
  char *tmp = NULL, *tmp2 = NULL, *val = NULL;
  char *tmp3 = {"for("};
  int len = 0;

  len = strlen(tmp3);
  WRITEFILE(tmp3, len);
  tmp = RemoveQ(&p->q); tmp2 = RemoveQ(&p->q); free(tmp2);
  tmp2 = RemoveQ(&p->q); 
  len = strlen(tmp);
  ALLOCATE(val, 1, len);
  strncpy(val, tmp, len);
  strcat(tmp, " = "); strcat(tmp, tmp2); free(tmp2);
  len = strlen(tmp);
  WRITEFILE(tmp, len);

  tmp = RemoveQ(&p->q); free(tmp);
  tmp = RemoveQ(&p->q);
  WRITEFILE(" ; ", 3);

  len = strlen(val);
  WRITEFILE(val, len);
  WRITEFILE(" <= ", 4);
  strcat(tmp, " ; ");
  len = strlen(tmp);
  WRITEFILE(tmp, len);
  free(tmp);

  strcat(val, "++){\n");
  len = strlen(val);
  WRITEFILE(val, len);

  free(val);

  return fp2;

}

FILE* writeSET(Prog *p, FILE *fp2)
{
  char *tmp = NULL;
  int len =0;;

  WRITEFILE("  ", 2);
  tmp = RemoveQ(&p->q);
  len = strlen(tmp);
  WRITEFILE(tmp, len);
  WRITEFILE(" = ", 3);
  free(tmp);
  fp2 = ReversePolish(p, fp2);
  WRITEFILE("\n", 1);

  return fp2;

}

int findlen(Prog *p)
{
  int i = 0;

  while(!strsame(";", p->q.s[p->q.front+ i])){
    i++;
  }

  return i;

}

/*function to put the polish expression in the right order
  in the c file - this is without brackets so will affect
  the functionality*/
FILE* ReversePolish(Prog *p, FILE* fp2)
{

  int len = 0, i = 0, flag = 0, n;
  char **str = NULL, *tmp = NULL;

  len = findlen(p); n = len;

  if((str = (char**)calloc(len, sizeof(char*))) == NULL){
    ERROR("Cannot Allocate Space", ErrAlloc);
  }

  while(flag != 1){
    tmp = RemoveQ(&p->q); 
    if(strsame(";", tmp)){
      flag = 1;
    }
    if(flag != 1){
      len = strlen(tmp);
      ALLOCATE(str[i], 1, len+1);
      strcpy(str[i], tmp); i++;
    }
    free(tmp);
  }
  i--;

  if(i > 1){
    swapPol(str, i);
  }
  writePolish(str, n, fp2);
 
  return fp2;

}

void writePolish(char **str, int n, FILE *fp2)
{
  int i, len = 0;

  for(i = 0; i < n; i++){
    len = strlen(str[i]);
    WRITEFILE(str[i], len);
    WRITEFILE(" ", 1);
    free(str[i]);
  }
  free(str);
  WRITEFILE(";", 1);

}

void swapPol(char **str, int i)
{
  int j = i, swap = 0;
  char *tmp=NULL;

  if(i == 2){
    SWAP; return;
  }
  do{
    while(i != 1){
       if((oper(str[i])) && (!oper(str[i-1])) &&
         (!oper(str[i-2]))){
        SWAP; swap = 1;
      } 
      else{
        swap = 0;
        if(oper(str[j])){
        swap = 1;
        }
      }    
      i--;
    }
    i = j;
  }while(swap != 0);

}

int oper(char* str)
{
  return((str[0] == '+') || (str[0] == '-') || 
  (str[0] == '*') || (str[0] == '/')) ? TRUE:FALSE;
}

int isvari(char* str)
{
  if((str[0] == '-') && ((str[1] >='A') && 
    (str[1] <= 'Z'))){
    return TRUE;
  }
  if(str[0] >= 'A' && str[0] <= 'Z'){
    return TRUE;
  }
  return FALSE;

}

/*function that finds variables at start to place at top
of file*/
FILE* testVariables(Prog *p, FILE *fp2)
{
  int i = 0, j = 0, k = 0, len = 0;
  char a[26][2];
  char *tmp = NULL;

  while(i != p->q.back){
    if((p->q.s[i][0] >= 'A' && p->q.s[i][0] <= 'Z')&&
      p->q.s[i][1] =='\0'){
      a[j][0] = p->q.s[i][0]  ;
      a[j][1] = '\0';  
      j++;
    }
    i++;
  }
  for(i = 0; i < j; i++){
    for(k = 0; k < i; k++){
      if(a[i][0] == a[k][0]){
        a[k][0] = '0';
      }
    }
  }
  for(k = 0; k < j; k++){
    if(a[k][0] !=  '0'){
      fwrite("  int ", sizeof(char), 6, fp2);
      tmp = a[k];
      len = strlen(tmp);
      WRITEFILE(tmp, len);
      WRITEFILE(";\n", 2);
    }
  }
  WRITEFILE("\n", 1);

  return fp2;

}
