#include "extens.h"

void syntaxError(Prog *p, int i, int err)
{
  insertError(p, i, err);
}
/*function to check if spaces were missing. Also takes
  into account \n and \r for compatibility*/
void checkspaces(Prog *p)
{
  int i;
  
  for(i = 0; i < p->count; i++){
    if((!strsame(" ", p->wrds[i])) && (!strsame("\n", 
      p->wrds[i])) && (!strsame("\r", p->wrds[i]))){
      if((p->wrds[i+1] != NULL) && ((!strsame(" ", 
        p->wrds[i+1])) && (!strsame("\n", p->wrds[i+1]))
        && (!strsame("\r", p->wrds[i+1])))){
        insertError(p, i, space);
      }
    }
  }
}
/*prints errors found from token stage*/
void printErrorsToken(Prog *p, Tokens *s)
{ 

  int i;

  for(i = 0; i <s->tokens-1; i++){
    if(p->errors[i] != NULL){
      errorStatement(p, i);
      printf("The problem was here %c, num %d\n\n",
      s->str[i], i);
      free(p->errors[i]);
      p->errors[i] = NULL;
    }
  }
  
}

void errorStatement(Prog *p, int i)
{
  switch(*p->errors[i]){
    case lower_case:
      ERROROUT("Warning : lower case letter. All letters\
 should be upper case in Turtle program. This is important\
 when programming as A can mean something very different to\
 a. Compiled o.k");
      break;
    case equals:
      ERROROUT("Warning : Missing = after :. Is Turtle \
program equals sign is :=. Compiled o.k");
      break;
    case not_known:
      ERROROUT("Warning : Unknown symbol in the program. \
Perhaps this was a typing mistake but please check again\
 the symbols for turtle program. Compiled ok");
      break;
    case space:
      ERROROUT("Warning : Missing Space. It is important to \
have spaces between instructions to make your code easier to\
 understand. Compiled ok");
      break;
    default:
      printf("TO DO - I need to be changed!");
  }
}
/*prints errors after syntax stage - seperated as need to
  store these differently*/
void printErrorSyntax(Prog *p)
{

  int i;

  for(i = 0; i < p->count; i++){
    if(p->errors[i] != NULL){
      errorSyntax(p, i);
      printf("The problems was here %s, num %d\n\n",
      p->wrds[i], i);
    }
  }
}

void errorSyntax(Prog *p, int i)
{
  switch(*p->errors[i]){
    case openBrack: case closeBrack:
      ERROROUT("Error : Program must open and close with\
 brackets { , }. This is important so that the program knows\
what symbols to read. Unable to compile");
      break;
    case brackets:
      ERROROUT("Error : Missing brackets. These are needed\
 with do loops to tell the program which parts of your code\
 to loop over. Unable to compile");
      break;
    case instruc:
      ERROROUT("Error : Expecting an instruction here. \
Instructions are needed to tell the program what to do. \
Unable to compile");
      break;
    case varnum:
      ERROROUT("Error : Expecting a number or variable. \
This value is needed to move or turn. Unable to compile");
      break;
    case scrBound:
      ERROROUT("Error : Lines being drawn outside of the\
 window. Unable to compile \
Suggest that FD or LT / RT values are reduced");
      break;
    case decimal:
      ERROROUT("Warning : too many decimals point in number,\
 there should only be one. Will compile but unpredicatable \
results");
      break;
    case variable:
      ERROROUT("Error : Missing a variable. Without this \
variable the program cannot function. Unable to compile");
      break;
    case doLoop:
      ERROROUT("Error : Do Loop is not properly written. It \
should appear as ' DO A from 1 TO 20 { '. Unable to compile");
      break;
    case neg:
      ERROROUT("Warning : Trying to do something in do loop\
 a negative amount of times. This is not possible suggest\
 changing second value to one higher than the first. Will \
compile but Do loop will not not happen.");
      break;
    case equal:
      ERROROUT("Warning : Missing full := sign. Will\
 compile but value will be set to 0");
      break;
    case polish:
      ERROROUT("Error : Polish expression is wrong. It \
should contain either just one variable or number, or one\
 more number than +, - , / , * sign. Unable to compile");
      break;
    case varset:
      ERROROUT("Error : Trying to use a variable that has\
 not yet been set. Try using the SET instruction before using\
 this. Unable to compile");
      break;
    case zero:
      ERROROUT("Warning : Trying to divide by 0. This is\
 mathematically impossible - you can't divide something by\
 nothing!. 0 has been reset to 0.1 but suggest changing \
input. Will compile");
      break;
    default:
      ERRORINPUT("Unknown ERROR Code", ErrUnknown);
  }

}

