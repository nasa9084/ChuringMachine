#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "./StringFunc.h"

#define FOPEN_ERR "File open error is occured."
#define MALLOC_ERR "Memory allocation error is occured."

#define MAXBUFF 50

typedef struct{
  String** program;
  char *symbols;
  int num_of_symbols, status;
  bool debugmode;
}Program;

typedef struct{
  char *str;
  int length;
}Tape;

void raiseError(String);
Program* makeProgram(FILE*);
Tape* makeTape(FILE*);
Program* printProgram(Program*);
Tape* printTape(Tape*);
Tape* runProgram(Program*, Tape*);


int main(int argc, char *argv[]){
  FILE *progfp, *inputfp;
  if(argc!=3){
    printf("Usage: %s <program file> <input file>", argv[0]);
    raiseError("");
  }

  if((progfp=fopen(argv[1], "r"))==NULL){
    raiseError(FOPEN_ERR);
  }

  if((inputfp=fopen(argv[2], "r"))==NULL){
    raiseError(FOPEN_ERR);
  }
  printTape( runProgram( makeProgram( progfp ), makeTape( inputfp )));

  fclose(progfp);
  fclose(inputfp);
  return 0;
}

void raiseError(String err){
  printf("%s\n",err);
  exit(1);
}

Program* makeProgram(FILE* fp){
  Program *prog;
  String tmp;
  Token *property;
  int i;
  //Programファイルの1行目にProgramのプロパティを記述
  //記号の数,状態の数,(デバッグモード)
  property = StringTokenize( StringFileScan(fp, 100), ",");

  if((prog=(Program*)malloc(sizeof(Program)))==NULL) return NULL;
  prog->num_of_symbols = atoi(property->strs[0]);
  prog->status = atoi(property->strs[1]);
  if((prog->program=(String**)malloc(sizeof(String*)*prog->num_of_symbols))==NULL){
    return NULL;
  }
  for(i=0; i<prog->num_of_symbols; i++){
    if((prog->program[i]=(String*)malloc(sizeof(String)*prog->status))==NULL){
      return NULL;
    }
  }
  if((prog->symbols=StringFileScan(fp, 100))==NULL){
    return NULL;
  }
  i=0;
  while((tmp=StringFileScan(fp, (prog->num_of_symbols)*4))!=NULL){
    prog->program[i++] = StringTokenize(tmp, ",")->strs;
  }
  if(property->size==3){
    prog->debugmode = true;
  }else{
    prog->debugmode = false;
  }
  return prog;
}

Tape* makeTape(FILE* fp){
  Tape *tape;
  if((tape=(Tape*)malloc(sizeof(Tape)))==NULL) return NULL;
  tape->length = atoi(StringFileScan(fp, 10));
  tape->str = StringFileScan(fp, tape->length);
  return tape;
}

Program* printProgram(Program* prog){
  int i, j;
  printf(" |");

  for(i=0; i<prog->num_of_symbols; i++){
    printf(" %c ", prog->symbols[i]);
    if(i!=prog->num_of_symbols-1){
      printf(",");
    }
  }
  printf("\n");
  printf("--");
  for(i=0; i<prog->num_of_symbols; i++){
    printf("---");
    if(i!=prog->num_of_symbols-1){
      printf("-");
    }
  }
  printf("\n");
  for(i=0; i<prog->status; i++){
    if(i!=0){
      printf("%d|", i);
    }else{
      printf("S|");
    }
    for(j=0; j<prog->num_of_symbols; j++){
      printf("%s", prog->program[i][j]);
      if(j!=prog->num_of_symbols-1){
        printf(",");
      }
    }
    printf("\n");
  }
  return prog;
}

Tape* printTape(Tape* tape){
  printf("Tape:: %s\n", tape->str);
  return tape;
}

Tape* runProgram(Program* prog, Tape* input){
  Tape *output = input;
  int currentState=0, currentPos=0, symbol, i;
  String transFunc;
  char readChar;

  printTape(input);


  while(currentState!=-1){
    readChar = input->str[currentPos];
    for(symbol=0; symbol<prog->num_of_symbols; symbol++){
      if(prog->symbols[symbol]==readChar){
        break;
      }
    }
    transFunc = prog->program[currentState][symbol];
    //debugmodeがオンなら経過を表示
    if(prog->debugmode){
      printf("CurrentState: %d / ", currentState);
      printf("CurrentPos: %d\n", currentPos);
      printTape(output);
      printf("       ");
      for(i=0; i<currentPos; i++){
        printf(" ");
      }
      printf("^\n");
      printf("TransFunc: %s\n\n", transFunc);
    }
    output->str[currentPos] = transFunc[0];
    if(transFunc[1]=='L'){
      if(currentPos!=0){
        currentPos--;
      }else{
        return NULL;
      }
    }else if(transFunc[1]=='R'){
      currentPos++;
    }else if(transFunc[1]=='N'){
    }else{
      return NULL;
    }
    if(transFunc[2]=='F'){
      currentState = -1;
    }else if(transFunc[2]=='S'){
      currentState = 0;
    }else{
      currentState = atoi(&transFunc[2]);
    }
  }

  return output;
}
