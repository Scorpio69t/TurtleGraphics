#include "extens.h"
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
  int flag = TRUE; 

  setWdwVaribles(&wdw);
  
  initialiseWindow(&wdw);

  if((p->wrds[0] == NULL) || (p->wrds[0][0] != '{')){
    insertError(p, 0, openBrack);
    flag = FALSE;
  }
  if(!strsame(p->wrds[p->count-1], "}")){
    insertError(p, p->count-1, closeBrack);
  }
  testBrackets(p);
  (flag == TRUE) ? (p->count = 1) : (p->count = 0);

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
    insertError(p, p->count-1, brackets);
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
      p->Qflag = 0;
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
  ADDERROR(instruc);
  NEXTWRD;

}

void applyInstr(Prog *p, Window *wdw)
{

  NEXTWRD;
  if(CURRENTWRD == NULL){
    return;
  }
  if(!isVarnum(p)){
    ADDERROR(varnum);
    return;
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
    testScreenEdge(wdw, p);
    drawLine(wdw, wdw->x, wdw->y, wdw->x1, wdw->y1);
    /*updates current position*/
    wdw->x = wdw->x1;
    wdw->y = wdw->y1;
  }
  if(p->Qflag != 1){
    ADDTOQ;
  }

  return;

}

/*test allows part of the line to be outside the boundaries
  to allow for patterns to fill the edges of the screen but
  creates Error if the full line to be drawn is outside of
  screen as this line would be unseen*/
void testScreenEdge(Window *wdw, Prog *p)
{

  if(((wdw->x1 && wdw->y1 < 0) || 
    (wdw->x1 && wdw->y1 > SIZE)) && 
    ((wdw->x && wdw->y < 0) ||(wdw->x && wdw->y > SIZE))){
      ADDERROR(scrBound);
      return;
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
  if(p->Qflag != 1){
    ADDTOQ;
  }

}


void applyLT(Prog *p, Window *wdw)
{

  wdw->angle %= FULLCIRCLE;
  wdw->angle += ((int)getNum(p) * -1);
  if(p->Qflag != 1){
    ADDTOQ;
  }

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
    ADDERROR(decimal);
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
    ADDERROR(variable);
  /*increased to move beyond do loop as is is not
    functional with the erro*/
    p->count += 4; 
  }
  ADDTOQ;
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
    return;
  }

  testFrom(p, tmp);
  testTo(p);
  n = ((int)getNum(p) + 1);
  NEXTWRD; 
  
  if(!strsame(CURRENTWRD, "{")){
    ADDERROR(doLoop);
  }
  
  applyDoLoop(p, wdw, (int)p->Var[tmp], n, tmp);
  p->Qflag = 0;
  /*resets angle to where it was prior to Do Loop*/
  wdw->angle = tmpAngle;
  InsertQ("}", &p->q);
  p->Qflag = 0;

}

void testFrom(Prog *p, int tmp)
{

  if(!strsame(CURRENTWRD, "FROM")){
    ADDERROR(doLoop);
    p->count += 3;
    return;
  }
  NEXTWRD;
  if(!isVarnum(p)){
    ADDERROR(varnum);
    p->count += 2;  
    return;
  }
  p->Var[tmp] = (int)getNum(p);
  ADDTOQ;
  NEXTWRD;

}

void testTo(Prog *p)
{

  if(!strsame(CURRENTWRD, "TO")){
    ADDERROR(doLoop);
    p->count++;
    return;
  }
  NEXTWRD;
  if(!isVarnum(p)){
    ADDERROR(varnum);
    return;
  }
  ADDTOQ;
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
    ADDERROR(neg);
    return;
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
    p->Qflag = 1;
    if((p->Var[tmp] >= n-1) || (CURRENTWRD == NULL)){
      return;
    }
   
    i++;
    p->Var[tmp] =+ i;
  }

}

double testSet(Prog *p)
{
  
  double tmp = 0;
  int flag = 0;

  if(CURRENTWRD == NULL || (!isVariable(p))){
    ADDERROR(variable);
    flag = 1;
  }
  if(p->Qflag != 1){
    ADDTOQ;
  }
  NEXTWRD;
  if(!strsame(CURRENTWRD, ":=")){
    ADDERROR(equal);
    flag = 1;
  }
  NEXTWRD;
  /*check to ensure that variable is in place to set to*/
  if(CURRENTWRD[0] == ';'){
    ADDERROR(polish);
    flag = 1;
  }
  tmp = testPolish(p);

  return(flag == 1) ? 0 : tmp;

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
    ADDERROR(varset);
    return;
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
  int flag = 0;

  initialiseStack(&s);

  while((CURRENTWRD[0] != ';') && (flag == 0)) {
    if((isVarnum(p)) && (flag == 0)){
      current = getNum(p);
      push(&s, current, p);
      if(p->Qflag == 0){
        InsertQ(CURRENTWRD, &p->q);
      }
      NEXTWRD;
    }
    else{
      if((!isOperator(CURRENTWRD[0])) && (flag == 0)){ 
        ADDERROR(polish);
        flag = 1;
      }
      if(flag == 0){
        if(p->Qflag == 0){
          InsertQ(CURRENTWRD, &p->q);
        }
        GET_FROM_STACK;
        current = calculateCurrent(p, tmp, current);
        push(&s, current, p);
        NEXTWRD;
      }
    }
    if(emptyStack(&s)){
      ADDERROR(polish);
      flag = 1;
    }
  } 
  if(flag == 0)
  current = pop(&s, p);
  if(!emptyStack(&s)){
    ADDERROR(polish);
    flag = 1;
  }
  if(p->Qflag != 1){
    InsertQ(CURRENTWRD, &p->q);
  }

  return(flag == 0) ? current : 0;   
      
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
        ADDERROR(zero);
        strcpy(CURRENTWRD, "0.1");
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

