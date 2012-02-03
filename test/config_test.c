
/**
 * @author Mogria
 * @version 0.1
 * @source TODO #####.c
 * @description TODO #####
 * @license GNU/GPL
 */


#include "../lib/config.h"

int main () {


  char filename[] = "config_test.cfg";

  puts("reading the config");
  struct assoc_array *config = read_config(filename);
  
  puts("printing whole array");
  assoc_print(config);

  puts("reading key");
  printf("key => %s\n", (char *)assoc_get(config, "key"));
  printf("test => %s\n", (char *)assoc_get(config, "test"));


  free_config(config);

  return 0;
} 
