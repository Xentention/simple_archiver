//
// Created by xenia on 06.03.23.
//

#ifndef SIMPLE_ARCHIVER_ARCHIVER_H
#define SIMPLE_ARCHIVER_ARCHIVER_H
#include "main.h"


int parse_dir(char *dir, DIR *directory, int o_file, int depth);            //Проходится по директории, архивируем находящиеся в ней файлы и подкаталоги
void add_dir_to_tar(int o_file, struct dirent *dir_ptr, int depth);         //архивирует подкаталог
int add_file_to_tar(char *fname, int o_file, int depth, char *true_name);	//Добавляет файл fname в архив arc. В заголовочном блоке указывается true_name, если это не пустая строка (используется для именования вложенных папок)
void add_header(char *fname, int o_file, int depth, char *true_name);       //Архивирует заголовок
void add_data_blocks(int o_file, int i_file);                               //Архивирует блок данных


#endif //SIMPLE_ARCHIVER_ARCHIVER_H
