#include "interp.h"

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
  functions. Also intialises Window so that this can be 
  passed to Code function*/
void test(Prog *p)
{

  Window wdw;

  setWdwVaribles(&wdw);
  
  initialiseWindow(&wdw);

  if((p->wrds[0] == NULL) || (p->wrds[0][0] != '{')){
    ERROR("Invalid Program, must start with opening \
    bracket - {", ErrInvalidProg);
  }
  if(p->wrds[p->count-1][0] != '}'){
    ERRORINPUT("Program must end with a } bracket",
    ErrCloseBrack);
  }
  testBrackets(p);
  p->count = 1;
  code(p, &wdw);
    
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
    ERROR("Missing matching brackets", ErrMatchBrk);
  }

}

/*recursive function to test each instruction. Window is
  updated here after each instruction. The functions which
  follow this all will end up back in this function after 
  a FD instruction. As this is the only instruction which 
  updates the screen it means each instruction is placed 
  on the screen as it is carried out*/
void code(Prog *p, Window *wdw)
{

  do{
    if((CURRENTWRD == NULL) || (CURRENTWRD[0] == '}')){
      return;
    }
    Neill_SDL_Events(wdw);
    /*ensures can escape the screen at any point, also 
      stops the recursion allowing early exit from 
      program*/
    if(wdw->finished == TRUE){
      return;
    }
    SDL_Delay(MSECDELAY);

    isInstruction(p, wdw);
    SDL_RenderPresent(wdw->renderer);
    SDL_UpdateWindowSurface(wdw->win);
    NEXTWRD;
    code(p, wdw);

  }while(wdw->finished == FALSE);
  
  freeWin(wdw);
  atexit(SDL_Quit);

}

void isInstruction(Prog *p, Window *wdw)
{

  double tmp = 0;
  int tmp2 = 0;

  if((strsame(CURRENTWRD, "FD")) ||
    (strsame(CURRENTWRD, "LT" )) || 
    (strsame(CURRENTWRD, "RT"))){
    applyInstr(p, wdw);
    return;
  }
  if(strsame(CURRENTWRD, "DO")){
    testDo(p, wdw);
    return;
  }
  if(strsame(CURRENTWRD, "SET")){
    NEXTWRD;
    tmp2 = CURRENTWRD[0] - 'A';
    tmp = testSet(p);
    /*takes value from set and places in array at the
      location of the char that setting to*/
    p->Var[tmp2] = tmp;
    return;
  }
  ERROR("Expecting Instruction", ErrInstruc);  

}

void applyInstr(Prog *p, Window *wdw)
{

  NEXTWRD;
  if((CURRENTWRD == NULL) || (!isVarnum(p))){
    ERROR("Expecting Varnum", ErrVar);
  }

  if(strsame(LASTWRD, "FD")){
    applyFD(p, wdw);
  }
  if(strsame(LASTWRD, "RT")){
    applyRT(p, wdw);
  }
  if(strsame(LASTWRD, "LT")){
    applyLT(p, wdw);
  }

}


void applyFD(Prog *p, Window *wdw)
{

  int dist = 0;

  if(strsame(LASTWRD, "FD")){
    /*casts to int as SDL requires integers for
      co-ordinates*/
    dist = (int)getNum(p);
    find_x_y(wdw, dist);
    testScreenEdge(wdw);
    drawLine(wdw, wdw->x, wdw->y, wdw->x1, wdw->y1);
    /*updates current position*/
    wdw->x = wdw->x1;
    wdw->y = wdw->y1;
  }

  return;

}

/*test allows part of the line to be outside the boundaries
  to allow for patterns to fill the edges of the screen but
  creates Error if the full line to be drawn is outside of
  screen as this line would be unseen*/
void testScreenEdge(Window *wdw)
{

  if(((wdw->x1 && wdw->y1 < 0) || 
    (wdw->x1 && wdw->y1 > SIZE)) && 
    ((wdw->x && wdw->y < 0) ||(wdw->x && wdw->y > SIZE))){
    ERRORINPUT("Line outside of Screen Boundaries",
    ErrScreenEdge);
  }

}

/*next two functions adjust angles. They ensure that the 
  angles remain within 360 degrees - without this the 
  angle continues to increase and could eventually go over 
  the size of the integer space*/
void applyRT(Prog *p, Window *wdw)
{

  wdw->angle %= FULLCIRCLE;
  wdw->angle += (int)getNum(p);

}


void applyLT(Prog *p, Window *wdw)
{

  wdw->angle %= FULLCIRCLE;
  wdw->angle += ((int)getNum(p) * -1);

}

/*converts polar co-ordinates to cartesian - first 
  calcuates radians of the angle so can be used with C
  functions*/
void find_x_y(Window *wdw, int dist)
{

  double r = PI_APPROX / COMBINED_ANGLES_TRI * wdw->angle;
  
  wdw->x1 = (int)(dist * cos(r));
  wdw->y1 = (int)(dist * sin(r));
  wdw->x1 += wdw->x;
  wdw->y1 += wdw->y;

}

uint8_t isVarnum(Prog *p)
{

  int i, n = (int)strlen(CURRENTWRD);
  uint8_t varnum = FALSE;
  uint8_t cnt = 0;
  /*allows for decimals and negatives*/
  for(i = 0; i < n; i++){
    if((isdigit(CURRENTWRD[i])) || (CURRENTWRD[i] == '.' ) 
      || (CURRENTWRD[i] == '-')){
      if(CURRENTWRD[i] == '.'){
        cnt++;
      }
      varnum = TRUE;
    }
    else{
      varnum = FALSE;
    }
  }  
  /*checks if variable set */
  if((isVariable(p) && CURRENTWRD[1] == '\0') || 
    (isNegVariable(p))){
    varnum = TRUE;
  }
 /*ensures that a negative number and not just operator -*/
  if(strsame(CURRENTWRD, "-")){
    varnum = FALSE;
  }

  if(cnt > 1){
    ERROR("Too many decimal points in Num", ErrDec);
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

void testDo(Prog *p, Window *wdw)
{

  NEXTWRD;
  if(CURRENTWRD == NULL || (!isVariable(p))){
    ERROR("Expecting Variable after instruction",
    ErrVariable);
  }

  testDoLoop(p, wdw);

}

/*tests do loop and sets variables for this to be 
  translated into a for loop to complete instructions*/
void testDoLoop(Prog *p, Window *wdw) 
{
  
  int tmp = CURRENTWRD[0] - 'A', n = 0, 
      tmpAngle = wdw->angle;

  NEXTWRD;
  if(CURRENTWRD == NULL){
    ERROR("Error in Do loop", ErrDo);
  }

  testFrom(p, tmp);
  testTo(p);
  n = ((int)getNum(p) + 1);
  NEXTWRD; 
  
  if(!strsame(CURRENTWRD, "{")){
    ERROR("Expecting {", ErrDoBrack);
  }
  
  applyDoLoop(p, wdw, (int)p->Var[tmp], n, tmp);
  /*resets angle to where it was prior to Do Loop*/
  wdw->angle = tmpAngle;

}

void testFrom(Prog *p, int tmp)
{

  if(!strsame(CURRENTWRD, "FROM")){
    ERROR("Error in Do loop, expecting FROM", ErrFrom);
  }
  NEXTWRD;
  if(!isVarnum(p)){
    ERROR("Missing Varnum in Do loop", ErrVarnum);
  }
  p->Var[tmp] = (int)getNum(p);
  NEXTWRD;

}

void testTo(Prog *p)
{

  if(!strsame(CURRENTWRD, "TO")){
    ERROR("Error in Do loop, expecting TO", ErrTo);
  }
  NEXTWRD;
  if(!isVarnum(p)){
    ERROR("Missing Varnum in Do loop", ErrVarnum);  
  }

  checkValues(p); 

}

void checkValues(Prog *p)
{

  double a, b;

  a = getNum(p);
  p->count -= 2;
  b = getNum(p);
  p->count += 2;

  if(b > a){
    ERROR("Attempt in do loop to carry out actions \
negatively\n", ErrVarnum);
  }


}

/*uses variables from DO loop in a for loop to call code
  function to complete instructions. This is recursive as
  still within the previous code function from the Do 
  instruction*/
void applyDoLoop(Prog *p, Window *wdw, int i, int n,
     int tmp)
{ 

  int flag = 0;

  NEXTWRD;
  flag = p->count;

  while(i <= n){
    if((CURRENTWRD[0] == '}') && (i != n)){
      p->count = flag;
    }
    code(p, wdw);
    if(p->Var[tmp] >= n-1){
      return;
    }
   
    i++;
    p->Var[tmp] =+ i;
  }

}

double testSet(Prog *p)
{
  
  double tmp = 0;

  if(CURRENTWRD == NULL || (!isVariable(p))){
    ERROR("Expecting Variable", ErrVariable);
  }
  NEXTWRD;
  if(!strsame(CURRENTWRD, ":=")){
    ERROR("Expecting :=", ErrPolish);
  }
  NEXTWRD;
  /*check to ensure that variable is in place to set to*/
  if(CURRENTWRD[0] == ';'){
    ERROR("Need value to set to", ErrPolBegin);
  }
  tmp = testPolish(p);
 
  return tmp;

}

/*checks for negative nums, returns current value held
  in variable array*/
double findCurrent(Prog *p)
{
  double tmp = 0;

  if(CURRENTWRD[0] == '-'){
    varSet(p, 1);
    tmp =  p->Var[CURRENTWRD[1] - 'A'];
    tmp *= -1;
    return tmp;
  }
  else{
    varSet(p, 0);
    return p->Var[CURRENTWRD[0] - 'A'];
  }
}

void varSet(Prog *p, int i)
{

  if(NOT_SET(p->Var[CURRENTWRD[i] - 'A'])){
    ERROR("Variable not set", ErrVarNotSet);
  }

}

/*checks whether variable or num and returns value of
  applicable one, using atof function for num*/
double getNum(Prog *p)
{

  return((isVariable(p)) || (isNegVariable(p))) 
        ? (findCurrent(p)) : (atof(CURRENTWRD));
}

/*uses stack to push values onto and pops from stack when 
  operator is found - calculates result and pushes back 
  onto stack until end of polish expression - function 
  needs to be long to ensure correct error checking and 
  clarity of what is happening*/
double testPolish(Prog *p)
{

  Stack s;
  double current = 0.0, tmp = 0.0;

  initialiseStack(&s);

  while(CURRENTWRD[0] != ';') {
    if(isVarnum(p)){
      current = getNum(p);
      push(&s, current);
      NEXTWRD;
    }
    else{
      if(!isOperator(CURRENTWRD[0])){ 
        ERROR("Expecting Operator, number or ;", 
        ErrPolish);
      }
      GET_FROM_STACK;
      current = calculateCurrent(p, tmp, current);
      push(&s, current);
      NEXTWRD;
    }
    if(emptyStack(&s)){
      ERROR("Missing Operator or Variable", ErrStackEmpt);
    }
  } 
  current = pop(&s);
  if(!emptyStack(&s)){
    ERROR("Wrong amount of variables and operators in Set",
    ErrStack);
  }

  return current;   
      
}

double calculateCurrent(Prog *p, double tmp, 
       double current)
{

  switch(CURRENTWRD[0]){
    case '+':{
      current = tmp + current;
      break;
    }
    case '-':{
      current = tmp - current;
      break;
    }
    case '/':{ 
      /*to ensure that not dividing by 0*/
      if(ZERO(current)){
        ERRORINPUT("Attempt to divide by Zero", 
        ErrZeroDiv); 
      }     
      current = tmp / current;
      break;
    }
    case '*':{
      current = tmp * current;
      break;
    }
  }
  return current;

}

uint8_t isOperator(char c)
{

  return((c == '-') || (c == '+') || (c == '/') 
        || (c == '*')) ? TRUE:FALSE;

}

