#include <stdlib.h>
#include <string.h>

struct assoc_element {
  const char *key;
  void *value;
};

struct assoc_array {
  unsigned int length;
  unsigned int position;
  struct assoc_element *elements;
  // pointer to dynamicly allocated memory using malloc() & realloc() & free() for the elements of the array
};


void assoc_array ( struct assoc_array *array );
void assoc_free ( struct assoc_array *array );
void *assoc_set ( struct assoc_array *array, const char *key, void *value );
void *assoc_get ( struct assoc_array *array, const char *key );
int assoc_exists ( struct assoc_array *array, const char *key );
int assoc_valid ( struct assoc_array *array );
unsigned int assoc_next ( struct assoc_array *array );
unsigned int assoc_prev ( struct assoc_array *array );
void assoc_rewind ( struct assoc_array *array );
struct assoc_element *assoc_element ( struct assoc_array *array );
void *assoc_current ( struct assoc_array *array );
const char *assoc_key ( struct assoc_array *array );

void assoc_array ( struct assoc_array *array ) {
  array->length = 0;
  array->position = 0;
  array->elements = malloc(0 * sizeof(struct assoc_element));
}

void assoc_free ( struct assoc_array *array ) {
  free(array->elements);
}

void *assoc_set ( struct assoc_array *array, const char *key, void *value ) {
  int ret;
  void *old_value = NULL;
  if ( ( ret = assoc_exists ( array, key ) ) == -1 ) {
    array->elements = realloc(array->elements, ++array->length * sizeof(struct assoc_element));
    array->elements[array->length - 1].key = key;
    array->elements[array->length - 1].value = value;
  } else {
    old_value = array->elements[ret].value;
    array->elements[ret].value = value;
  }
  return old_value;
}

void *assoc_get ( struct assoc_array *array, const char *key ) {
  int ret = assoc_exists ( array, key );
  return ret == -1 ? NULL : array->elements[ret].value;
}

int assoc_exists ( struct assoc_array *array, const char *key ) {
  int ret = -1;
  unsigned int old_pos = array->position;
  for ( assoc_rewind ( array ); assoc_valid ( array ) && ret == -1; assoc_next ( array ) ) {
    if ( strcmp ( assoc_key ( array ), key ) == 0 ) {
      ret = array->position;
    }
  }
  array->position = old_pos;
  return ret;
}

int assoc_valid ( struct assoc_array *array ) {
  return array->position < array->length && array->position >= 0 ? 1 : 0;
}

unsigned int assoc_next ( struct assoc_array *array ) {
  return ++array->position;
}

unsigned int assoc_prev ( struct assoc_array *array ) {
  return --array->position;
}

void assoc_rewind ( struct assoc_array *array ) {
  array->position = 0;
}

struct assoc_element *assoc_element ( struct assoc_array *array ) {
  struct assoc_element *ret = NULL;
  if ( assoc_valid ( array ) ) {
    ret = &array->elements[array->position];
  }
  return ret;
}

void *assoc_current ( struct assoc_array *array ) {
  struct assoc_element *element = assoc_element ( array );
  return element == NULL ? NULL : element->value;
}

const char *assoc_key ( struct assoc_array *array ) {
  struct assoc_element *element = assoc_element ( array );
  return element == NULL ? NULL : element->key;
}

void assoc_print ( struct assoc_array *array ) {
  puts ("\nArray (" );
  for ( assoc_rewind ( array ); assoc_valid ( array ); assoc_next ( array ) ) {
    printf("\t'%s' => %p\n", assoc_key(array), assoc_current(array));
  }
  puts(")\n");
}
