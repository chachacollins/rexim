#include "rexim.h"
#include <stdio.h>

ArrayList(string_t);

ArrayList(char);

int main(void) {
  Arena *arena = arena_init(1024);
  string_t buffer = string_from("This fucking sucks");
  ;
  printf("%s and length is %zu\n", buffer.items, buffer.len);
  string_t slice = get_string_slice(&buffer, 7, 11, arena);
  string_t slice2 = get_string_slice(&buffer, 1, 5, arena);
  printf("%s and length is %zu\n", slice.items, slice.len);
  printf("%s and length is %zu\n", slice2.items, slice2.len);

  ArrayList_char list = ArrayList_char_init(arena);

  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'a', arena);
  ArrayList_char_append(&list, 'b', arena);
  printf("The char is %c and the len is %zu\n", list.items[list.len - 1],
         list.len);
  string_t file_name = read_file_to_string("README.md", arena);
  printf("%s\n %zu\n", file_name.items, file_name.len);
  arena_destroy(arena);
}
