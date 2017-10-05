/* This program takes all the characters from the file
 as chars into a string. It then chnages these to tokens
 in the prog struct to be futher parsed. This is done so 
 that errors in the file can be acknowledged at this point
 but not exited on. A record of these is kept for further 
 output to the user. This is more in line with the
 tokenization stage of a compiler rather than taking whole
 strings. Some magic nums left in place when used to
 increment counts for clarity*/

#include "extens.h"

Prog* Tokenize(Prog *p, Tokens *s)
{
  int i = 0;
  IsLower(s, p);
  while(i <= s->tokens){
    i = testNumToken(s, i, p, 0);
    i = testVar(s, i, p);
    i = testOther(s, i, p);
    i++;
  }
  return p;
}

/*simply adds regular tokens - but tested those that could
  be equals or negative variables*/
int testOther(Tokens *s, int i, Prog *p)
{
  char tmp[STRSIZE] = {0};
  
  tmp[0] = s->str[i];
  if(!isdigit(tmp[0]) && (tmp[0] < 'A' || tmp[0] > 'Z')){
    switch(s->str[i]){
      case '{': case '}': case '+': case '/': case '*':
      case ';': case ' ': case '\n':{
        tmp[1] = '\0';
        insertString(tmp, p, p->count);
        break;
      }
      case ':':
        i = testEquals(s, i, p, tmp);
        break;
      case '-':
        i = testNegative(s, i, p, tmp);
        break;
      default:
        if(i != s->tokens){
          insertError(p, i, not_known);
        }
    } 
  }  
 
  return i;

}

int testNegative(Tokens *s, int i, Prog *p, char *tmp)
{

  if(isdigit(s->str[i+1])){
    i = testNumToken(s, i, p, '-');
  }
  else if(s->str[i+1] >= 'A' && s->str[i+1] <= 'Z'){
    tmp[1] = s->str[i+1];
    tmp[2] = '\0';
    insertString(tmp, p, p->count);
    i++;
  }
  else{
    tmp[1] = '\0';
    insertString(tmp, p, p->count);
  }

  return i;

}

/*test whether := input correctly and adjusts string to be 
  correct*/
int testEquals(Tokens *s, int i, Prog *p, char *tmp)
{

  int flag = 0;
  if(s->str[i+1] != '='){
    flag = 1;
    tmp[1] = '\0';
  }
  else{
    tmp[1] = '=';
    tmp[2] = '\0';
    i++;
  }
  insertString(tmp, p, p->count);
  if(flag == 1){
    insertError(p, i, equals);
  }

  return i;
}

/*calls functions to determine if char is a instruction
  or variable*/
int testVar(Tokens *s, int i, Prog* p)
{
  char tmp[STRSIZE] = {0};

  tmp[0] = s->str[i];

  if(s->str[i] >= 'A' && s->str[i] <= 'Z'){
    switch(s->str[i]){
      case 'F':
        i = testF(s, i, p, tmp);
        break; 
      case 'L': case 'R': 
        i = testLR(s, i, p, tmp);
        break;
      case 'S':
        i = testS(s, i, p, tmp);
        break;
      case 'D': case 'T':
        i = testDT(s, i, p, tmp);
        break;
      default: 
        tmp[1] = '\0';
        insertString(tmp, p, p->count);
    }
  }
      
  return i;

}

/*following tests check that each string is an 
  instruction or variable. magic numbers left in
  for clarity of positions for instructions*/
int testDT(Tokens *s, int i, Prog *p, char *tmp)
{

  if(s->str[i+1] == 'O'){
    tmp[1] = 'O'; tmp[2] = '\0';
    i++;
    if(s->str[i-1] == 'D'){
      s->DoFlag++;
    }
  }
  else{
    tmp[1] = '\0';
  }
  insertString(tmp, p, p->count);

  return i;

}

int testS(Tokens *s, int i, Prog *p, char *tmp)
{
  if((s->str[i+1] == 'E') && (s->str[i+2] == 'T')){
    tmp[1] = 'E'; tmp[2] = 'T'; tmp[3] = '\0';
    i += 2;
  }
  else{
    tmp[1] = '\0';
  }
  insertString(tmp, p, p->count);

  return i;

}

int testLR(Tokens *s, int i, Prog *p, char *tmp)
{

  if(s->str[i+1] == 'T'){
    tmp[1] = 'T';
    tmp[2] = '\0';
    i++;
  }
  else{
    tmp[1] = '\0';
  }
  insertString(tmp, p, p->count);

  return i;

}

int testF(Tokens *s, int i, Prog *p, char *tmp)
{

  if(s->str[i+1] == 'D'){
    tmp[1] = 'D'; tmp[2] = '\0'; 
    i++;
  }
  else if((s->str[i+1] == 'R') && (s->str[i+2] == 'O') &&
  (s->str[i+3] == 'M')){
    tmp[1] = 'R'; tmp[2] = 'O'; tmp[3] = 'M';
    tmp[4] = '\0';
    i += 3;
  }
  else{
    tmp[1] = '\0';
  }
  insertString(tmp, p, p->count);

  return i;

}


void IsLower(Tokens *s, Prog *p)
{

  int i = 0;
  while(i <= s->tokens){
    if(s->str[i] >= 'a' && s->str[i] <= 'z'){
      s->str[i] = toupper(s->str[i]);
      insertError(p, i, lower_case);
    }
    i++;
  }
}

/*tests for both negative and decimal values*/
int testNumToken(Tokens *s, int i, Prog *p, int flag)
{

  char tmp[STRSIZE] = {0};
  int n = 0;
  if(flag == '-'){
    tmp[0] = '-';
    n = 1; 
    i++;
  }

  if(isdigit(s->str[i])){
    while((isdigit(s->str[i])) || (s->str[i] == '.')){   
      tmp[n] = s->str[i];
      n++; i++;
    }
    tmp[n] = '\0';
    insertString(tmp, p, p->count); 
  }

  return(tmp[0] == '-')? --i:i;
}

void getWords(FILE *file, Tokens *s)
{

  char c;
  int len = INITIALSIZE;

  s->str = InitialiseString(s->str, len);

  while(fscanf(file, "%c", &c) == TRUE){
    if(s->tokens == len - 1){
      s->str = increaseWrdSpace(s, len);
      len = DOUBLE(len);
    }
    s->str[s->tokens++] = c;
  }

}

char* increaseWrdSpace(Tokens* s, int len)
{
  
  char *tmp = NULL;

  int i = DOUBLE(len);

  tmp = InitialiseString(tmp, i);

  strncpy(tmp, s->str, len);

  free(s->str);

  return tmp;

}


