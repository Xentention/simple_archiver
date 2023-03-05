//
// Created by xenia on 05.03.23.
//

#ifndef SIMPLE_ARCHIVER_MAIN_H
#define SIMPLE_ARCHIVER_MAIN_H

#define BLOCK_SIZE 512	//Размер блока
#define MAX_NAME_LENGTH 255	//Максимальный размер имени файла
#define PERMISSION 00666	//Разрешения

int make_archive(char *fname, char *dir, int depth);	//Архивирует файлы из папки dir в архив fname уровня depth

int unpack_archive(char *fname, char *dir, int depth);	//Распаковывает архив

#endif //SIMPLE_ARCHIVER_MAIN_H
