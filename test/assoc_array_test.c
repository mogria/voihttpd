
#include <stdio.h>
#include "../lib/assoc_array.h"


int main ( void ) {
  struct assoc_array array;
  int value = 5;
  assoc_array ( &array );
  int value2 = 15;
  assoc_set ( &array, "voidness", (void*)&value );
  assoc_set ( &array, "voidness", (void*)&value2 );
  assoc_set ( &array, "voidness2", (void*)&value2 );
  assoc_set ( &array, "what the fuck", (void*)&value );
  printf ( "and the number is .... : %i expected : %i \n", *((int*)assoc_get ( &array, "voidness" )), value2);
  printf ( "and the number is .... : %i expected : %i \n", *((int*)assoc_get ( &array, "voidness2" )), value2);
  printf ( "and the number is .... : %i expected : %i \n", *((int*)assoc_get ( &array, "what the fuck" )), value);
  printf ( "position : %i expected %i \n", array.position, 0);
  assoc_prev ( &array );
  printf ( "position : %i expected %i \n", array.position, -1);
  printf ( "res: %p expected %p \n", assoc_current(&array), NULL);
  assoc_print ( &array );
  return 0;
}
