/*
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
*/
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint8_t *buffer;
  size_t buffer_size;
  size_t current_offset;
  size_t prev_offset;
} Arena;

static inline Arena *arena_init(size_t size) {
  if (!size) {
    fprintf(stderr, "Size must be > 0\n");
    exit(1);
  }

  Arena *arena = (Arena *)malloc(sizeof(Arena));
  if (!arena) {
    fprintf(stderr, "Could not alloc arena\n");
    exit(1);
  }

  arena->buffer = (uint8_t *)calloc(1, size);
  if (!arena->buffer) {
    free(arena);
    fprintf(stderr, "Could not alloc arena buffer\n");
    exit(1);
  }

  arena->buffer_size = size;
  arena->current_offset = 0;
  arena->prev_offset = 0;
  return arena;
}
static inline void *arena_alloc(Arena *arena, size_t size, size_t alignment) {
  if (!size || !arena) {
    fprintf(stderr, "Size or arena not provided\n");
    exit(1);
  }

  if (size > SIZE_MAX / 2) {
    fprintf(stderr, "Allocation size too large\n");
    exit(1);
  }

  size_t mask = alignment - 1;
  size_t misalignment = arena->current_offset & mask;
  size_t adjustment = (misalignment > 0) ? (alignment - misalignment) : 0;
  size_t aligned_offset = arena->current_offset + adjustment;

  if (aligned_offset + size > arena->buffer_size) {
    size_t new_size = arena->buffer_size * 2;
    uint8_t *new_buffer = (uint8_t *)calloc(1, new_size);
    if (!new_buffer) {
      fprintf(stderr, "Failed to reallocate arena\n");
      exit(1);
    }
    memcpy(new_buffer, arena->buffer, arena->buffer_size);
    free(arena->buffer);
    arena->buffer = new_buffer;
    arena->buffer_size = new_size;
  }

  arena->prev_offset = arena->current_offset;
  arena->current_offset = aligned_offset + size;
  return arena->buffer + aligned_offset;
  return arena->buffer + aligned_offset;
}
static inline void arena_destroy(Arena *arena) {
  if (arena) {
    free(arena->buffer);
    free(arena);
  }
}

typedef struct {
  char *items;
  size_t len;
} string_t;

static inline string_t get_string_slice(string_t *str, size_t begin, size_t end,
                                        Arena *arena) {
  if (begin >= end) {
    fprintf(stderr, "Invalid range: begin (%zu) must be less than end (%zu)\n",
            begin, end);
    exit(1);
  }
  if (begin > str->len || end > str->len) {
    fprintf(stderr, "Invalid range %zu - %zu for string of length %zu\n", begin,
            end, str->len);
    exit(1);
  }

  size_t len = end - begin;
  string_t slice;
  slice.items =
      (char *)arena_alloc(arena, sizeof(char) * (len + 1), _Alignof(char));
  if (!slice.items) {
    perror("Failed to allocate memory for string slice");
    exit(1);
  }

  memcpy(slice.items, str->items + begin, len);
  slice.items[len] = '\0';
  slice.len = len;
  return slice;
}

static inline string_t string_from(char *str) {
  string_t string;
  string.items = str;
  string.len = strlen(str);
  return string;
}

#define ArrayList(T)                                                           \
  typedef struct {                                                             \
    T *items;                                                                  \
    size_t len;                                                                \
    size_t capacity;                                                           \
  } ArrayList_##T;                                                             \
                                                                               \
  ArrayList_##T ArrayList_##T##_init(Arena *arena) {                           \
    size_t initialCapacity = 1024;                                             \
    ArrayList_##T list;                                                        \
                                                                               \
    list.items =                                                               \
        (T *)arena_alloc(arena, sizeof(T) * initialCapacity, _Alignof(T));     \
    list.len = 0;                                                              \
    list.capacity = initialCapacity;                                           \
    return list;                                                               \
  }                                                                            \
  void ArrayList_##T##_append(ArrayList_##T *arraylist, T data,                \
                              Arena *arena) {                                  \
    if (arraylist->len >= arraylist->capacity) {                               \
      void *new_buffer =                                                       \
          (T *)arena_alloc(arena, arraylist->capacity * 2, _Alignof(T));       \
      memcpy(new_buffer, arraylist->items, arraylist->len);                    \
      arraylist->items = new_buffer;                                           \
      arraylist->capacity *= 2;                                                \
    }                                                                          \
    arraylist->items[arraylist->len] = data;                                   \
    arraylist->len++;                                                          \
  }

/*
  FILE IO STUFF
*/

static inline size_t file_length(FILE *file) {
  fseek(file, 0L, SEEK_END);
  size_t len = ftell(file);
  fseek(file, 0L, SEEK_SET);
  return len;
}
static inline string_t read_file_to_string(const char *filepath, Arena *arena) {
  FILE *file = fopen(filepath, "rb");
  if (!file) {
    fprintf(stderr, "Could not open file: %s\n", filepath);
    exit(1);
  }

  size_t len = file_length(file);
  char *buffer =
      (char *)arena_alloc(arena, sizeof(char) * (len + 1), _Alignof(char));

  size_t read = fread(buffer, 1, len, file);
  if (read != len) {
    fprintf(stderr, "Could not read all the file to buffer\n");
    exit(1);
  }

  buffer[len] = '\0';
  string_t str = {.items = buffer, .len = len};

  fclose(file);
  return str;
}

static inline string_t write_to_file(const char *filepath,
                                     const char *content) {
  FILE *file = fopen(filepath, "w");
  fprintf(file, "%s", content);
  fclose(file);
}
