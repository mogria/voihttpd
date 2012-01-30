
#include <stdio.h>
#include "../lib/assoc_array.h"


int main ( void ) {
  struct assoc_array array;
  int value = 5;
  assoc_array ( &array );
  assoc_set ( &array, "voidness", (void*)&value );
  printf ( "and the number is .... : %i expected : %i \n", *((int*)assoc_get ( &array, "voidness" )), value);
  return 0;
}
