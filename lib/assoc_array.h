/** Author: Mogria <m0gr14@gmail.com>
 *
 * Example of use:
 *
 * int value = 132;
 * int value2 = 15;
 * struct assoc_array myarray;
 *
 * assoc_array(&myarray); // gcgcinitialize the array
 * 
 * assoc_set ( &myarray, "myvalue", (void *)&value ); // gcgcset an element
 * *(int *)assoc_get ( &myarray, "myvalue" ); // gcgc123
 *
 * assoc_set ( &myarray, "my_other_value", (void *)&value2 ); // gcgcset an element
 *
 * for ( assoc_rewind( &myarray ); assoc_valid ( &myarray ); assoc_next ( &myarray ) ) {
 *   printf ( "key: %c value: %i\n", assoc_key ( &myarray ), *(int *)assoc_current ( &myarray ) );
 * }
 * // gcprints
 * // gcgckey: myvalue value: 132
 * // gcgckey: myvalue value: 15
 *
 * assoc_free(&myarray); // gcgcfree the allocated memory for the array elements
 */

#ifndef _ASSOC_ARRAY_H
#define _ASSOC_ARRAY_H 1

#include <stdio.h>  /* gcputs, printf */
#include <stdlib.h> /* gcfree, malloc, realloc */
#include <string.h> /* gcstrcmp */

/* gcgcan element of the array */
struct assoc_element {
  const char *key;
  void *value;
};


/* gcgcthe struct for the array */
struct assoc_array {
  unsigned int length;
  unsigned int position;
  struct assoc_element *elements;
  /* gcgcpointer to dynamicly allocated memory using malloc() & realloc() & free() for the elements of the array */
};


/* gcdeclrations */
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

/* gcgcinitialize an array */
void assoc_array ( struct assoc_array *array ) {
  array->length = 0;
  array->position = 0;
  array->elements = malloc(0 * sizeof(struct assoc_element));
}

/* gcgcfree the allocated memory of the array */
void assoc_free ( struct assoc_array *array ) {
  free(array->elements);
}

/* gcgcset the specified key to the specified value */
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

/* gcgcget the value of the specified key */
void *assoc_get ( struct assoc_array *array, const char *key ) {
  int ret = assoc_exists ( array, key );
  return ret == -1 ? NULL : array->elements[ret].value;
}

/* gcgcchecks if the given key exists */
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

/* gcgcchecks of the position in the array is valid ( needed for iterating ) */
int assoc_valid ( struct assoc_array *array ) {
  return array->position < array->length && array->position >= 0 ? 1 : 0;
}

/* gcgcmoves to the next element in the array */
unsigned int assoc_next ( struct assoc_array *array ) {
  return ++array->position;
}

/* gcgcmoves to the previous element in the array */
unsigned int assoc_prev ( struct assoc_array *array ) {
  return --array->position;
}

/* gcgcsets the position in tbe array to 0 */
void assoc_rewind ( struct assoc_array *array ) {
  array->position = 0;
}

/* gcgcget the array element of the current position in the array */
struct assoc_element *assoc_element ( struct assoc_array *array ) {
  struct assoc_element *ret = NULL;
  if ( assoc_valid ( array ) ) {
    ret = &array->elements[array->position];
  }
  return ret;
}

/* gcgcreturn the value of the current element */
void *assoc_current ( struct assoc_array *array ) {
  struct assoc_element *element = assoc_element ( array );
  return element == NULL ? NULL : element->value;
}

/* gcgcreturn the key of the current element */
const char *assoc_key ( struct assoc_array *array ) {
  struct assoc_element *element = assoc_element ( array );
  return element == NULL ? NULL : element->key;
}

/* gcgcprint out the whole array */
void assoc_print ( struct assoc_array *array ) {
  puts ("\nArray (" );
  for ( assoc_rewind ( array ); assoc_valid ( array ); assoc_next ( array ) ) {
    printf("\t'%s' => %p\n", assoc_key(array), assoc_current(array));
  }
  puts(")\n");
}

#endif /* _ASSOC_ARRAY_H */
