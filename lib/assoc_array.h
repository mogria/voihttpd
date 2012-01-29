
struct assoc_element {
  char *key;
  void *value;
}

struct assoc_array {
  unsigned int length;
  unsigned int position;
  struct assoc_element *elements;
}


void assoc_array ( struct assoc_array *array ) {
  array->length = 0;
  array->position = 0;
  array->elements = NULL;
}
