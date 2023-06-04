
#include "s21_grep.h"

// strcpy (копирует строку внутрь массива)
// argc-счетчик(кол-во аргументов) argv-массив строк(описание команд)(указатель
// на 0 и ндекс каждого аргумента)

void Grep(int argc, char **argv) {
  if (argc > 1) {
    char *temp = NULL;
    options conf = {0};

    for (int y = ReadGlags(argc, argv, &conf, &temp); y < argc; y += 1)
      GrepFile(argv[y], conf, temp);  ///Ищем в нашей строчке флаги и записываем

    if (temp != NULL) free(temp);
  }
}
int main(int argc, char **argv) {
  Grep(argc, argv);
  return 0;
}

int ReadGlags(int argc, char **argv, options *conf, char **template) {
  for (int CurrentFlag = 0;
       (CurrentFlag = getopt(argc, argv, "e:f:iocnlhsv")) != (-1);) {
    switch (CurrentFlag) {
      case 'i':
        conf->i = true;
        break;
      case 'o':
        conf->o = true;
        break;
      case 'c':
        conf->c = true;
        break;
      case 'n':
        conf->n = true;
        break;
      case 'l':
        conf->l = true;
        break;
      case 'h':
        conf->h = true;
        break;
      case 's':
        conf->s = true;
        break;
      case 'v':
        conf->v = true;
        break;
      case 'e':
        set_conf_e(conf, template, optarg);
        break;
      case 'f':
        set_conf_f(conf, template, optarg);
        break;
    }
  }
  if ((conf->e || conf->f) == 0) {
    create_temp_late(template, argv[optind]);
    optind += 1;  // После флага идет шаблон или файл с шаблоном поэтому +1
  }
  setup_conf(conf, argc);
  return optind;
}

void GrepFile(char *path, options conf, char *template) {
  FILE *file = fopen(path, "r");
  if (file != NULL) {
    // 0-начало таблицы ansii
    for (char CurrentFlag = '0'; (CurrentFlag = getc(file)) != EOF;) {
      char *line = calloc(256, 1);  //(кол-во элементов, вес char - 1 байт)
      conf.NumberLine += 1;
      int length = 0;
      //\0 - конец строки
      for (line[length] = '\0'; CurrentFlag != EOF && CurrentFlag != '\n';
           CurrentFlag = getc(file)) {
        line[length] = CurrentFlag;
        line[length += 1] = '\0';
        if (length % 255 == 0) {
          line = ChangeValueMemory(line, length + 256);  //добавили память
        }
      }
      PrintMainData(line, &conf, template, path);
      free(line);
    }
    PrintAuxData(conf, path);
    fclose(file);
  } else if (conf.s == 0) {
    fprintf(stderr, ERROR, path);
  }
}

void *ChangeValueMemory(void *str, int size) {
  char *aux = realloc(str, size);  //изменяет величину выделенной памяти
  return aux;
}

void PrintAuxData(options conf, char *path) {
  if (conf.c) {
    if (conf.l) {
      conf.CountFiles > 1
          ? printf("%s:1\n", path)
          : printf("1\n");  //если больше одного выводим название файла и enter
    } else {
      if (conf.CountFiles > 1)
        printf("%s:", path);  //если больше одного выводим название файла
      printf("%i\n", conf.CountMatches);  //выводим кол-во
    }
  }
  if (conf.l && conf.CountMatches) printf("%s\n", path);
}

void PrintMainData(char *line, options *conf, char *template, char *path) {
  regex_t regex;
  // REG_ICASE - без учета регистра
  if (regcomp(&regex, template, conf->i ? REG_ICASE : REG_EXTENDED) == 0) {
    if (regexec(&regex, line, 0, NULL, 0) == conf->v) {
      conf->CountMatches += 1;
      if ((conf->c || conf->l) == false) {
        if (conf->CountFiles > 1 && !(conf->h))
          printf("%s:", path);  //название файлов, если несколько
        if (conf->n) printf("%i:", conf->NumberLine);  //номер строки
        if (!conf->o)
          printf("%s\n", line);
        else
          printf_o(regex, line, *conf);
      }
    }
    regfree(&regex);
  }
}

void printf_o(regex_t regex, char *line, options conf) {
  while (regexec(&regex, line, 0, NULL, 0) == conf.v) {
    char *aux = (char *)calloc(strlen(line) + 1, 1);
    strcpy(aux, line);  //копирует данные из строки line в aux
    int end = strlen(line);
    while (regexec(&regex, aux, 0, NULL, 0) == conf.v) {
      end--;
      aux[strlen(aux) - 1] = 0;
    }
    aux[strlen(aux)] = line[strlen(aux)];
    int start = 0;
    while (regexec(&regex, aux, 0, NULL, 0) == conf.v && strlen(aux) > 0) {
      start++;
      int j = 0;
      while (aux[j] != 0) {
        aux[j] = aux[j + 1];
        j++;
      }
    }
    start--;
    int i = strlen(aux);
    while (i != 0) {
      aux[i] = aux[i - 1];
      i--;
    }
    aux[0] = line[start];
    printf("%s\n", aux);
    free(aux);
    i = start + 1;
    while (line[i] != 0) {
      line[i - start - 1] = line[i];
      i++;
    }
    line[i - start - 1] = 0;
  }
}

void set_conf_f(options *conf, char **template, char *optarg) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    for (char CurrentFlag = '0';
         (CurrentFlag = getc(file)) != EOF;) {  //следующий символ не конец
      int length = 0;
      char *line = calloc(256, 1);

      for (line[length] = '\0'; CurrentFlag != EOF && CurrentFlag != '\n';
           CurrentFlag = getc(file)) {
        line[length] = CurrentFlag;
        line[length += 1] = '\0';
        if (length % 255 == 0)
          line = (char *)ChangeValueMemory(line, length + 256);
      }

      if (!(conf->e || conf->f))
        conf->f = create_temp_late(template, line);
      else
        add_temp_late(template, line);
      free(line);
    }
    fclose(file);
  } else if (conf->s == false) {
    fprintf(stderr, ERROR, optarg);
  }
}

void set_conf_e(options *conf, char **template, char *optarg) {
  if (!(conf->e || conf->f))
    conf->e = create_temp_late(template, optarg);
  else
    add_temp_late(template, optarg);
}

void setup_conf(options *conf, int argc) {
  if (conf->o && (conf->l || conf->v || conf->c))
    conf->o = false;  //обнуляем -о
  conf->CountFiles = argc - optind;
}

int create_temp_late(char **str, char *optarg) {
  *str = calloc(strlen(optarg) + 1, 1);
  if (*str) strcpy(*str, optarg);  //скопировали optarg в str
  return str ? 1 : 0;
}

void add_temp_late(char **str, char *optarg) {
  *str = ChangeValueMemory(*str, strlen(*str) + strlen(optarg) + 2);
  if (*str) strcat(strcat(*str, "|"), optarg);
  //соединяет в цепочку str1 и копию строки str2
}
