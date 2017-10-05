#include "extens.h"


int main (int argc, char **argv)
{

  FILE *fp = NULL, *fp2 = NULL;
  Tokens s;
  Prog *p = NULL;
  char c;

  /*following gets the input into tokens*/
  fp = fopen(argv[1], "r");
  checkInput(argc, fp);
  s.tokens = 0; s.DoFlag = 0;
  getWords(fp, &s);
  fclose(fp);  
  /*checks syntax of tokens and interps program*/

  p = initialiseProg(s.tokens, p);
  p = Tokenize(p, &s);
  p->DoFlag = s.DoFlag;
  adjustProg(p);
  printErrorsToken(p, &s);
  free(s.str);
  test(p);

  /*checks for errors, printing to screen. If no serious
  errors prints to c file*/
  printErrorSyntax(p);
  
  printf("%d\n", p->DoFlag);

  if(checkErrors(p) == TRUE){
    fp2 = MakeCFile(fp2, p);
    fclose(fp2);
    printf("Would like to compile your program?\nY for \
yes  N for no:");
    if(scanf("%c", &c) != 1){
      ERROR("Needed a Y or N", ErrInput);
    }
    Compile(c);
  }

  freeProg(p);

  return 0;

}





