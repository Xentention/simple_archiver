//
// Created by xenia on 05.03.23.
//
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../headers/unarchiver.h"

int unpack_archive(char *fname, char *dir, int depth) {	//Распаковывает архив
    //Откроем архив
    int i_file = open(fname, O_RDONLY);
    if (i_file == -1)	{
        printf("Файл не создан");
        return 1;
    }

    //Проверим наличие папки
    DIR *directory = opendir(dir);
    if (directory == NULL) {
        //Создадим папку
        if (mkdir(dir, S_IRWXU|S_IRWXG|S_IRWXO) == -1) {
            printf("Не удается открыть/создать папку %s", dir);
            close(i_file);
            return 2;
        }
    }
    closedir(directory);
    if (chdir(dir) == -1)
        perror("");

    return unpacking(i_file, depth);
}

int unpacking(int i_file, int depth){
    //Начинаем распаковывать файлы
    unsigned long i_size;
    int i_depth;
    char i_name[MAX_NAME_LENGTH];
    char a;
    while (read(i_file, i_name, MAX_NAME_LENGTH) == MAX_NAME_LENGTH) {
        read(i_file, &i_size, sizeof(long));	//Считывание размера файла
        read(i_file, &i_depth, sizeof(int));	//Считывание глубины залегания файла

        for (int i = 0; i < BLOCK_SIZE - MAX_NAME_LENGTH - sizeof(int) - sizeof(long); i++) {
            read(i_file, &a, 1);	//Считывание заголовочного блока полностью
        }

        //Проверяем, является ли считанный файл файлом или вложенной папкой
        if (i_depth == depth+1) {
            unpack_dir(i_file, i_size, i_name, depth);
        } else if (i_depth == depth) {
            unpack_file(i_file, i_size, i_name);
        } else {
            printf("Ошибка архива - несовпадение глубин\n");
            close(i_file);
            return 3;
        }
    }
    close(i_file);
    chdir("..");	//Возвращаемся на уровень выше
    return 0;
}

void unpack_dir(int i_file, unsigned long i_size, char *i_name, int depth){
    printf("Вложенная папка\n");
    //Распаковка вложенного архива
    unpack_file(i_file, i_size, ".included_archive");
    unpack_archive(".included_archive", i_name, depth+1);
    remove(".included_archive");
}

int unpack_file(int i_file, unsigned long i_size, char *true_name)
{
    char i_block[BLOCK_SIZE];
    int n_file = open(true_name, O_CREAT|O_WRONLY, PERMISSION);	//Создание файла
    if (n_file == -1)
        printf("Не получается создать папку");

    printf("Распаковка %s\n", true_name);
    //Заполнение файла
    for (int i = 0; i < i_size; ) {
        //Считываем блок
        read(i_file, i_block, BLOCK_SIZE);
        printf("%i/%lu - \n", i, i_size);
        //Записываем блок
        if ((i + BLOCK_SIZE) > i_size) {
            write(n_file, i_block, i_size-i);	//Дописываем файл
            break;
        } else {
            write(n_file, i_block, BLOCK_SIZE);
            i += BLOCK_SIZE;
        }
    }
    close(n_file);
    return 0;
}
