#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR "s21_grep: %s: No such file or directory\n"

typedef struct {
  bool e;  //Шаблон
  bool f;  //Шаблон из файла
  bool i;  //Игнорирует различия регистра
  bool v;  //Только не совпадающее
  bool c;  //Выводит только количество совпадающих строк
  bool l;  //Выводит только совпадающие файлы
  bool n;  //Нумерует
  bool h;  //Выводит совпадающие строки
  bool s;  //Подавляет сообщения об ошибках о несуществующих или нечитаемых
           //файлах
  bool o;  //Непустые совпадающие строки
  int NumberLine;
  int CountFiles;
  int CountMatches;
} options;

void Grep(int argc, char **argv);  //запрос в main
int ReadGlags(int argc, char **argv, options *conf, char **template);
void GrepFile(char *path, options conf, char *template);
void *ChangeValueMemory(void *str, int size);
void PrintAuxData(options conf, char *path);
void PrintMainData(char *line, options *conf, char *template, char *path);
void printf_o(regex_t regex, char *line, options conf);
void set_conf_f(options *config, char **template, char *optarg);
void set_conf_e(options *conf, char **template, char *optarg);
void setup_conf(options *conf, int argc);
int create_temp_late(char **str, char *optarg);
void add_temp_late(char **str, char *optarg);
#endif

//-ggdb //Порождает отладочную информацию в родном формате