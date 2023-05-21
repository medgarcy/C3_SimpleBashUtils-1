
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "catFlags.h"

typedef struct {
  bool is_non_blank;  //нумерует только непустые строки
  bool mark_end;  //также отображает символы конца строки как $
  bool number_all;  //нумерует все выходные строки
  bool squeeze;  //сжимает несколько смежных пустых строк
  bool tab;  //также отображает табы как ^I
  bool print_non_printable;  // show непечатаемые символы, кроме t, end
} Flags;

Flags read_flags(int argc, char *argv[]) {
  struct option longOptions[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0},
  };
  int current_flag = getopt_long(argc, argv, "beEvnstT", longOptions, NULL);
  Flags flags = {false, false, false, false, false, false};
  for (; current_flag != -1;
       current_flag = getopt_long(argc, argv, "beEvnstT", longOptions, NULL)) {
    switch (current_flag) {
      break;
      case 'b':
        flags.is_non_blank = true;
        break;
      case 'e':
        flags.mark_end = true;
        flags.print_non_printable = true;
        break;
      case 'E':
        flags.mark_end = true;
        break;
      case 'n':
        flags.number_all = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.tab = true;
        flags.print_non_printable = true;
        break;
      case 'T':
        flags.tab = true;
    }
  }
  return flags;
}

void CatFile(FILE *file, Flags flags, const char *table[static 256]) {
  int c = 0;
  int last;
  bool squeeze = false;
  int line_number = 0;
  last = '\n';

  (void)flags;
  while (fread(&c, 1, 1, file) > 0) {
    if (last == '\n') {
      if (flags.squeeze && c == '\n') {
        if (squeeze) continue;
        squeeze = true;
      }

      else
        squeeze = false;

      if (flags.is_non_blank) {
        if (c != '\n') printf("%6i\t", ++line_number);
      }

      else if (flags.number_all) {
        printf("%6i\t", ++line_number);
      }
    }
    if (!*table[c])
      printf("%c", '\0');
    else
      printf("%s", table[c]);
    last = c;
  }
}

void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]) {
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       ++filename) {
    if (**filename == '-') continue;
    FILE *file = fopen(*filename, "rb");

    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    CatFile(file, flags, table);
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = read_flags(argc, argv);
  const char *table[256];
  CatSetTable(table);
  if (flags.mark_end) CatSetEndl(table);
  if (flags.tab) CatSetTab(table);
  if (flags.print_non_printable) CatSetNonPrintable(table);
  Cat(argc, argv, flags, table);
}
