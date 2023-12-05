#include "Array.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void array_init(Array *self, int capacity, int element_size) {
  self->data = (void **)malloc(capacity * sizeof(void *));
  self->capacity = capacity;
  self->count = 0;
  self->element_size = element_size;
}
/*
void array_push(Array *self, void *value)
{
  if (self->count == self->capacity)
  {
    self->capacity = self->capacity * 2;
    self->data = realloc(self->data, self->capacity * sizeof(void *));
  }
  self->data[self->count] = value;
  self->count += 1;
}

*/
void * array_push(Array *self) {
  void * value = NULL;
  if (self->count == self->capacity) {
    self->capacity = self->capacity * 2;
    self->data = realloc(self->data, self->capacity * sizeof(void *));
  }
  value = malloc(self->element_size);
  self->data[self->count] = value;
  self->count += 1;
  return value;
}


void array_free(Array *self)
{
  for (int i = 0; i < self->count; i++)
  {
    free(self->data[i]);
    self->data[i] = NULL;
  }
  free(self->data);
  self->data = NULL;
}

void *array_get(Array *self, int pos) { return self->data[pos]; }

void init_dictionary(Array *self, FILE *dict)
{
  char *buff;
  char line[256];
  while (fgets(line, 101, dict) != NULL)
  {
    line[strcspn(line, "\n")] = 0;
    buff = strtok(line, ",");
    array_push(self, strdup(buff));
    buff = strtok(NULL, ",");
    array_push(self, strdup(buff));
  }
}
// form 0 pro preklad z angl do cestiny 1 kdyz naopak
void array_write_line(char *buff, Array *self, FILE *write, int form)
{
  while (buff != NULL)
  {
    bool found = false;
    for (int i = form; i < self->count; i += 2)
    {
      if (strcmp((char *)array_get(self, i), buff) == 0)
      {
        fprintf(write, "%s", (char *)array_get(self, i + 1 - 2 * form));
        found = true;
        break;
      }
    }
    if (!found)
    {
      fprintf(write, "<unknown>");
    }
    buff = strtok(NULL, " ");
    if (buff != NULL)
    {
      fprintf(write, " ");
    }
  }
}