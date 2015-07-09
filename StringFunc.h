#ifndef STRING_FUNC_H
#define STRING_FUNC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define swap(a,b) ((a!=b) && (a^=b^=a^=b))

typedef char* String;
typedef struct{
  int size;
  String *strs;
}Token;

size_t StringLength(String);
String StringAlloc(size_t);
String StringCpy(String);
String StringSlice(String, int, int);
String StringConcat(String, String);
String StringReverse(String);
String StringFileScan(FILE*, int);
Token* StringTokenize(String, String);

size_t StringLength(String str){
  return strlen(str);
}

String StringAlloc(size_t size){
  return (String)malloc(sizeof(char)*size);
}

String StringCpy(String str){
  return strcpy((String)malloc(sizeof(char)*(StringLength(str)+1)), str);
}

String StringSlice(String str, int from, int to){
  String cpy=StringCpy(str), retStr;
  if(from>to){
    swap(from, to);
  }
  if(to>=0){
    cpy[to] = '\0';
  }else{
    cpy[StringLength(cpy)+to] = '\0';
  }

  if(from>=0){
    retStr = StringCpy(&cpy[from]);
  }else{
    retStr = StringCpy(&cpy[StringLength(str)+from]);
  }
  free(cpy);
  return retStr;
}

String StringConcat(String str1, String str2){
  String retStr = ( String )malloc(sizeof(char)*(StringLength(str1)+StringLength(str2)+1));
  strcpy(retStr, str1);
  strcpy(&retStr[StringLength(str1)], str2);
  return retStr;
}

String StringReverse(String str){
  String retStr = ( String )malloc(StringLength(str)+1);
  int i;
  for(i=0; i<(int)StringLength(str); i++){
    retStr[i] = str[StringLength(str)-i-1];
  }
  retStr[StringLength(str)] = '\0';
  return retStr;
}

String StringFileScan(FILE* fp, int max){
  String tmp;
  if((tmp=StringAlloc(max))==NULL) return NULL;
  if((fscanf(fp, "%s", tmp))==EOF) return NULL;
  return tmp;
}

Token* StringTokenize(String str, String delimiter){
  String *tokens;
  String tp;
  int token_num = 1, i=0;
  Token *token;
  tp = strtok(StringCpy(str), delimiter);
  while(strtok(NULL, delimiter)!=NULL){
    token_num++;
  }
  free(tp);
  tokens = ( String* )malloc(sizeof(String)*token_num);
  tokens[i++] = strtok(StringCpy(str), delimiter);
  while((tokens[i]=strtok(NULL, delimiter))!=NULL){i++;}
  token = ( Token* )malloc(sizeof(Token));
  token->size = i;
  token->strs = tokens;
  return token;
}

#endif
