#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 100

typedef struct {
  char* stack;
  int top;
} Stack;

int isempty(Stack* stack);
int isfull(Stack* stack);
char peek(Stack* stack);
char pop(Stack* stack);
char push(Stack* stack, char data);