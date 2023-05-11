
#include <errno.h>
#include <getopt.h>  //разбирает аргументы командной строки
// int(значение, используемое, чтобы понять какой флаг передан) getopt_long(int
// argc, char * const argv[],
//           const char *optstring,
//           const struct option *longopts(структура, передающая длинные флаги),
//           int *longindex(сюда сохраняется результат);
#include <stdbool.h>  //определяет следующие четыре макроса: bool,true,false,_bool_true_false_are_defined //for struct
// int argc(переменная, в которую передано количество аргументов), char
// *argv(переданы сами аргументы в виде массива строк. Массив длина которого
// равна 1 переменной, и каждый аргумент которого является строкой, в которую
// помещается аргумент. в 0 элемент помещается имя файла/имя файла и путь)
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
  bool print_non_printable;  //показывать непечатаемые символы, кроме табуляции
                             //и конца строки.
} Flags;

Flags read_flags(int argc, char *argv[]) {
  struct option longOptions[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0},
  };
  int current_flag = getopt_long(argc, argv, "beEvnstT", longOptions,
                                 NULL);  // getopt передает флаг через цикл
  Flags flags = {false, false, false,
                 false, false, false};  //переменная, хранящая флаги, не поймаем
                                        //флаг=он не был установлен
  for (; current_flag != -1;
       current_flag = getopt_long(argc, argv, "beEvnstT", longOptions, NULL)) {
    switch (current_flag)  //обработка того что возвращает getopt
    {
      break;
      case 'b':
        flags.is_non_blank =
            true;  //.=в переменную задаем параметр из структуры
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

void CatFile(FILE *file, Flags flags,
             const char *table[static 256])  //чтение из файла
{
  int c = 0;  //текущие символы
  int last;   //последний символ
  bool squeeze = false;
  int line_number = 0;
  last = '\n';

  (void)flags;
  while (fread(&c, 1, 1, file) >
         0)  //байты, кол-во символов, файл из которого читаем)
  {
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
    if (!*table[c])  //проверка на ноль
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
    FILE *file = fopen(*filename, "rb");  //для виндоус

    if (errno) {  //для определения вида ошибки
      fprintf(stderr, "%s", argv[0]);  //Стандартная ошибка
      perror(*filename);
      continue;
    }
    CatFile(file, flags, table);
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = read_flags(argc, argv);
  const char *table[256];  //таблица
  CatSetTable(table);      //передаем таблицу

  if (flags.mark_end) CatSetEndl(table);
  if (flags.tab) CatSetTab(table);
  if (flags.print_non_printable) CatSetNonPrintable(table);
  Cat(argc, argv, flags, table);
}
