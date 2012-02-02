#ifndef _CONFIG_H
#define _CONFIG_H 1
#include "assoc_array.h"
#include <stdio.h> // fopen, fclose, fgets

#define BUFSIZE 80

struct assoc_array *read_config(char *filename) {
  static short initialized = 0;
  static struct assoc_array files;
  static struct assoc_array *configs;
  static char **contents;

  int pos;
  FILE *file;
  int c;
  char buf[BUFSIZE] = {};
  if (!initialized ) {
    assoc_array ( &files );
    initialized = 1;
    configs = malloc ( sizeof ( struct assoc_array ) * 0 );
    contents = malloc ( sizeof ( char * ) * 0 );
  }

  if ( ( pos = assoc_exists ( &files, filename ) ) != -1) {
    return (struct assoc_array *)&files.elements[pos];
  } else {
    assoc_set(&files, filename, NULL);
    file = fopen ( filename, "r" );
    if ( file != NULL ) {
      configs = realloc ( configs, sizeof ( struct assoc_array ) * files.length );
      struct assoc_array *array = &configs[files.length - 1];
      
      contents[files.length - 1] = malloc ( sizeof ( char ) * 80 );
      int offset = 0;
      while ( fgets(buf, BUFSIZE, file) != NULL) {
        int len = strlen(buf);
        strncpy(contents[files.length - 1] + offset, buf, len + 1);
        offset += len;
        contents[files.length - 1] = realloc ( contents[files.length -1], sizeof ( char ) * ( offset + 80 ) );
        // what if an other pointer is returned??? copy the shit arround xD
      }

      printf("%s", contents[files.length - 1]);
      // @TODO: read the file and insert directives into array
      fclose ( file );

    }
  }
}

void free_config(struct assoc_array *array) {
  for(assoc_rewind ( array ); assoc_valid ( array ); assoc_next ( array ) ) {
    free ( assoc_current ( array ) );
    // @TODO: probably needs recode when implemented read_config()
  }

  assoc_free ( array );
}
#endif /* _CONFIG_H */

