#include <stdio.h>

typedef struct {
  void **data;
  int count;
  int capacity;
  int element_size;
} Array;

void array_free(Array *self);
void array_init(Array *self, int capacity, int element_size);
//void array_push(Array *self, void *value);
void *array_push(Array *self);
void *array_get(Array *self, int pos);
void init_dictionary(Array *self, FILE *dict);
void array_write_line(char *buff, Array *self, FILE *write, int form);