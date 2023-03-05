//
// Created by xenia on 06.03.23.
//

#ifndef SIMPLE_ARCHIVER_UNARCHIVER_H
#define SIMPLE_ARCHIVER_UNARCHIVER_H

#include "main.h"

int unpacking(int i_file, int depth);                                           //Непосредственно распаковывает архив
void unpack_dir(int i_file, unsigned long i_size, char *i_name, int depth);     //распаковывает папку в архиве
int unpack_file(int i_file, unsigned long i_size, char *true_name);	                        //Распаковывает один файл из архива.


#endif //SIMPLE_ARCHIVER_UNARCHIVER_H
