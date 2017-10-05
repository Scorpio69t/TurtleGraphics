/*main program file*/

#include "parser.h"

int main(int argc, char **argv)
{

  FILE *file = NULL;
  Prog *p = NULL;

  file = fopen(argv[1], "r");
  checkInput(argc, file);
  p = initialiseProg(INITIALINPUT, p);
  p = readWords(file, p);
  fclose(file);

  test(p);  
  freeProg(p);

  return 0;

}


/*starts the testing of code by ensuring that there is 
  something to test and the it opens with a bracket. Also 
  calls function to scan through code and ensure the 
  correct amount of matching brackets - further bracket 
  testing for specific position carried out through later 
  functions*/
void test(Prog *p)
{

  if((p->wrds[0] == NULL) || (p->wrds[0][0] != '{')){
    ERRORINPUT(
    "Invalid Program, must start with opening bracket - {",
    ErrInvalidProg);
  }

  testBrackets(p);
  p->count = 1;
  code(p);
    
}

void testBrackets(Prog *p)
{

  int i, cnt = 0;

  for(i = 0; i < p->count; i++){
    if(p->wrds[i][0] == '{'){
      cnt++;
    }
    if(p->wrds[i][0] == '}'){
      cnt--;
    }
  }

  if(cnt != 0){
    ERRORINPUT("Missing matching brackets", ErrMatchBrk);
  }

}

/*recursive function that call itself after each type of
  instruction is parsed*/
void code(Prog *p)
{

  if(CURRENTWRD[0] == '}'){
    return;
  }

  isInstruction(p);
  
  NEXTWRD;
  code(p);

}

/*called within code function to check each instruction 
  and call specific function for each instruction set*/
void isInstruction(Prog *p)
{

  if(CURRENTWRD[0] == '}'){
    NEXTWRD;
    return;
  }

  if((strsame(CURRENTWRD, "FD")) ||
     (strsame(CURRENTWRD, "LT" )) ||
     (strsame(CURRENTWRD, "RT"))){
    testFDLTRT(p);
    return;
  }

  if(strsame(CURRENTWRD, "DO")){
    testDo(p);
    return;
  }

  if(strsame(CURRENTWRD, "SET")){
    testSet(p);
    return;
  }

  ERROR("Expecting Instruction", ErrInstruc);  

}

void testFDLTRT(Prog *p)
{

  NEXTWRD;
    
  if((p->wrds[p->count] == NULL) || (!isVarnum(p))){
    ERROR("Expecting Varnum", ErrVar);
  }

  return;

}

/*checks for both Variable and numbers, account for 
  negatives and decimals*/
uint8_t isVarnum(Prog *p)
{

  int i, n = (int)strlen(p->wrds[p->count]);
  uint8_t varnum = FALSE;

  for(i = 0; i < n; i++){
    if((isdigit(CURRENTWRD[i])) || (CURRENTWRD[i] == '.') 
      || (CURRENTWRD[i] == '-')){
      varnum = TRUE;
    }
  }  

  if(((isVariable(p)) && (CURRENTWRD[1] == '\0')) 
    || (isNegVariable(p))){
    varnum = TRUE;
  }

  return varnum;
}

uint8_t isVariable(Prog *p)
{
  return((CURRENTWRD[0] >= 'A') && (CURRENTWRD[0] <= 'Z'))
        ? TRUE:FALSE;
}

/*2 left here rather than hash defined to make it clear 
  which part of string is being accessed*/
uint8_t isNegVariable(Prog *p)
{
  return(CURRENTWRD[0] == '-' && (CURRENTWRD[1] >= 'A' && 
         CURRENTWRD[1] <= 'Z') && CURRENTWRD[2] == '\0') 
         ? TRUE:FALSE;
}

/*called from isInstruction function - seperated from test 
  do loop to avoid conflict with recursion*/
void testDo(Prog *p)
{
  NEXTWRD;
  if(CURRENTWRD == NULL || (!isVariable(p))){
    ERROR("Expecting Variable", ErrVariable);
  }

  testDoLoop(p);

}

/*recursive function to check that Do loop has correct 
  instructions in the right order*/
void testDoLoop(Prog *p) 
{

  static int cnt = 0;

  NEXTWRD;
  if(CURRENTWRD == NULL){
    ERROR("Error in Do loop", ErrDo);
  }

  if((strsame(CURRENTWRD, "{")) && (cnt == DOLENGTH)){
    cnt = 0;
    return;
  }

  if((!testFrom(p, cnt)) && (!testTo(p, cnt))){
    ERROR("Error in Do loop", ErrDoLoop);
  } 
  
  cnt++;
  NEXTWRD;

  if(!isVarnum(p)){
    ERROR("Missing Varnum in Do loop", ErrVarnum);
  }

  cnt++;
  testDoLoop(p);

}

/*below two functions to test TO and FROM in correct 
  position - 2 left in for clarity of which position 
  TO should appear in */
uint8_t testFrom(Prog* p, int cnt)
{
  return((strsame(CURRENTWRD,"FROM")) && (cnt == 0))
        ? TRUE:FALSE;
} 

uint8_t testTo(Prog *p, int cnt)
{
  return((strsame(CURRENTWRD,"TO")) && (cnt == 2))
        ? TRUE:FALSE;
} 

void testSet(Prog *p)
{
  
  NEXTWRD;
  if(CURRENTWRD == NULL || (!isVariable(p))){
    ERROR("Expecting Variable", ErrVariable);
  }

  NEXTWRD;
  if(!strsame(CURRENTWRD, ":=")){
    ERROR("Expecting :=", ErrSet); 
  }

  NEXTWRD;
  if(CURRENTWRD == NULL){
    ERROR("Expecting Polish expression", ErrMissPol);
  }
  else{
    testPolish(p);
  }  

}

/*recursive function to test polish expression. Allows 
  expressions that have no value such as simply ; according
  to grammar rules*/
void testPolish(Prog *p)
{

  if(CURRENTWRD[0] == ';'){ 
    return;
  } 

  if((isOperator(CURRENTWRD[0])) || (isVarnum(p))){
    NEXTWRD;
    testPolish(p);
  }
  else{
    ERROR("Missing Operator, number or ; before", 
    ErrPolish); 
  }

}

uint8_t isOperator(char c)
{
  return((c == '-') || (c == '+') || (c == '/') || 
        (c == '*')) ? TRUE:FALSE;
}



