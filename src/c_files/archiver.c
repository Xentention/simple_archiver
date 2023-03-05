//
// Created by xenia on 05.03.23.
//
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../headers/archiver.h"


int make_archive(char *fname, char *dir, int depth) {
    //Создает файл будущего архива в соответствующей директориии
    int o_file = open(fname, O_CREAT | O_WRONLY, PERMISSION);
    if (o_file == -1) {
        printf("Не получается создать архив: %s", fname);
        return 1;
    }
    perror("");

    //Открывает текущую архивируемую директорию
    DIR *directory = opendir(dir);  // open dir
    if (directory == NULL) {
        printf("Не получается открыть директорию: %s", dir);
        return 2;
    }
    return parse_dir(dir, directory, o_file, depth);
}

//Проходимся по директории, архивируем находящиеся в ней файлы и подкаталоги
int parse_dir(char *dir, DIR *directory, int o_file, int depth) {
    chdir(dir);    //Переходим в эту директорию
    struct dirent *dir_ptr;
    struct stat stat_file;
    dir_ptr = readdir(directory);

    while (dir_ptr != NULL) {
        if (strcmp((*dir_ptr).d_name, ".") != 0 &&
            strcmp((*dir_ptr).d_name, "..") != 0) {    //Пропускаем точки и двойные точки

            printf("> %s", (*dir_ptr).d_name);
            //Проверяем на то, является ли файл папкой
            lstat((*dir_ptr).d_name, &stat_file);
            if (S_ISDIR(stat_file.st_mode)) {
                add_dir_to_tar(o_file, dir_ptr, depth);     //если да, архивируем как папку
            } else {
                add_file_to_tar((*dir_ptr).d_name, o_file, depth, "");    //если нет - как файл
            }
        }
        dir_ptr = readdir(directory);    //Переходим к следующему файлу
    }

    chdir("..");                //Возвращаемся на уровень выше
    closedir(directory);
    if (close(o_file) == -1) {
        return 5;
    }

    return 0;
}

//архивирует папку
void add_dir_to_tar(int o_file, struct dirent *dir_ptr, int depth) {
    printf(" - папка.\n");
    make_archive(".tempttar", (*dir_ptr).d_name, depth + 1);                     //Архивируем подкаталоги
    add_file_to_tar(".tempttar", o_file, depth + 1, (*dir_ptr).d_name);    //Записываем промежуточный файл в архив
    remove(".tempttar");    //Удаляем временный файл
}

//архивирует файлы
int add_file_to_tar(char *fname, int o_file, int depth, char *true_name) {
    printf(" - файл.\n");
    //Архивируем заголовок
    if (strlen(fname) > MAX_NAME_LENGTH) {
        printf("Слишком длинное название файла\n");
        return 1;
    }
    add_header(fname, o_file, depth, true_name);

    //Архивируем блок данных
    int i_file = open(fname, O_RDONLY);
    if (i_file == -1) {
        printf("Не удалось открыть файл %s", fname);
        return 2;
    }
    add_data_blocks(o_file, i_file);

    close(i_file);
    return 0;
}

//Архивируем заголовок
void add_header(char *fname, int o_file, int depth, char *true_name) {
    unsigned long size;
    struct stat stats;
    lstat(fname, &stats);
    size = stats.st_size;
    printf("Размер: %lu\n", size);
    if (strcmp("", true_name) == 0) {
        if (write(o_file, fname, MAX_NAME_LENGTH) != MAX_NAME_LENGTH)    //Запись имени
            perror("");
    } else
        write(o_file, true_name, MAX_NAME_LENGTH);    //Запись имени папки
    write(o_file, &size, sizeof(long));               //Запись размера файла
    write(o_file, &depth, sizeof(int));               //Запись глубины залегания файла

}

//Архивируем блок данных
void add_data_blocks(int o_file, int i_file) {
    char a = 0;
    for (int i = 0; i < BLOCK_SIZE - MAX_NAME_LENGTH - sizeof(int) - sizeof(long); i++)
        write(o_file, &a, 1);    //Заполняем блок из BLOCK_SIZE байт до конца
    //Блоки данных
    unsigned char block[BLOCK_SIZE];
    int count = read(i_file, block, BLOCK_SIZE);
    while (count == BLOCK_SIZE) {
        write(o_file, block, BLOCK_SIZE);
        count = read(i_file, block, BLOCK_SIZE);
    }
    if (count != 0) {
        write(o_file, block, count);
        for (int i = 0; i < BLOCK_SIZE - count; i++)    //Дописываем блок до BLOCK_SIZE байт
            write(o_file, &a, 1);
    }
}