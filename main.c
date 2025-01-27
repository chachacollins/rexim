#include "rexim.h"
#include <stdio.h>
#include <string.h>

ArrayList(string_t);

ArrayList(char);

int main(void) {
  Arena *arena = arena_init(1024);
  string_t buffer = {.items = "Hello world", .len = strlen(buffer.items)};
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

  ArrayList_string_t items = ArrayList_string_t_init(arena);
  arena_destroy(arena);
}

int hello(void)
{
  printf("Hello world\n");
}
