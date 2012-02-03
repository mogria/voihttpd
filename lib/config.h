#ifndef _CONFIG_H
#define _CONFIG_H 1
#include "assoc_array.h"
#include <stdio.h> // fopen, fclose, fgets

#define BUFSIZE 80

struct assoc_array *read_config(char *filename);
void parse_config ( char *content, struct assoc_array *array);

struct assoc_array *read_config(char *filename) {
  static short initialized = 0;
  static struct assoc_array files;
  static struct assoc_array *configs;
  static char **contents;

  int pos;
  FILE *file;
  int c;
  char buf[BUFSIZE] = {};

  if ( filename == NULL ) {
    free(configs);
    int i;
    for ( i = 0; i < files.length; i++) {
      free(contents[i]);
    }
    free(contents);
    assoc_free(&files);
    initialized = 0;
  } else {
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

        parse_config(contents[files.length - 1], array);
        
        fclose ( file );

      }
    }
    return &configs[files.length - 1];
  }
}

void parse_config ( char *content, struct assoc_array *array) {
  int i;
  int length = strlen(content);
  short commented = 0;
  char *current_idx = content;
  short is_key = 1;
  for (i = 0; i < length; i++ ) {
    // printf("c: %c\n", content[i]);
    if ( content[i] == '\n' ) {
      // puts("is a line break, setting current char to 0, use next char as index");
      is_key = 1;
      commented = 0;
      if ( content[i + 1] != '\0' ) {
        current_idx = &content[i + 1];
      }
      content[i] = '\0';
    } else if ( content[i] == '#' || commented ) {
      // puts("is a hash sign set it all to \\0 until line break");
      commented = 1;
      content[i] = '\0';
    } else if ( content[i] == '=' ) {
      // printf("found an equal sign\n");
      if(is_key) {
        content[i] = '\0';
        if ( content[i + 1] != '\0' ) {
          // printf("  set element %s in array to %s", current_idx, &content[i + 1]);
          assoc_set(array, current_idx, &content[i + 1]);
        }
      }
      is_key = 0;
    } else if ( content[i] == ' ' || content[i] == '\t' || content[i] == '\v' ) {
      if(is_key) {
        content = '\0';
      }
    }
  }
}

void free_config(struct assoc_array *array) {
  assoc_free ( array );
  read_config(NULL);
}
#endif /* _CONFIG_H */

