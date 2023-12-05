#include "Stack.h"

int isempty(Stack* stack) {
  if (stack->top == -1)
    return 1;
  else
    return 0;
}

int isfull(Stack* stack) {
  if (stack->top == MAXSIZE)
    return 1;
  else
    return 0;
}

char peek(Stack* stack) { return stack->stack[stack->top]; }

char pop(Stack* stack) {
  int data;

  if (!isempty(stack)) {
    data = stack->stack[stack->top];
    stack->top = stack->top - 1;
    return data;
  } else {
  }
}

char push(Stack* stack, char data) {
  if (!isfull(stack)) {
    stack->top = stack->top + 1;
    stack->stack[stack->top] = data;
  } else {
  }
}