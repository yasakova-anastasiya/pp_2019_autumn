// Copyright 2019 Kurakin Mikhail
#include "../../../modules/task_1/kurakin_m_count_diff_ch/count_diff_ch.h"
#include <mpi.h>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <random>
#include <string>
#include <vector>

std::string getRandomString(int size) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz!`~№#;$%^&*()-_=+><{}[]";
    int alphanumLen = std::strlen(alphanum);
    std::string str(size, 0);
    for (int i = 0; i < size; ++i) {
        str[i] = alphanum[rand() % (alphanumLen - 1)];
    }
    return str;
}

int getCountDiffChPar(std::string str1, std::string str2) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int step =  str1.length() / size;
    int start = rank * step;
    int end = start + step;
    int localCounter = 0;
    int globCounter = 0;
    bool uniq;
    for (int i = start; i < end; i++) {
        uniq = true;
        for (int j = 0; j < static_cast<int>(str2.length()); j++) {
            if (str1[i] == str2[j]) {
                uniq = false;
            }
        }
        if (uniq == true) {
            localCounter++;
        }
    }
    int newEnd = 0;
    MPI_Reduce(&end, &newEnd, 1, MPI_INT, MPI_MAX, 0,
                MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        int remainChAmount = str1.length() % size;
         if (remainChAmount != 0) {
            start = newEnd;
            newEnd = str1.length();
             for (int i = start; i < newEnd; i++) {
                uniq = true;
                for (int j = 0; j < static_cast<int>(str2.length()); j++) {
                    if (str1[i] == str2[j]) {
                        uniq = false;
                    }
                }
                if (uniq == true) {
                    localCounter++;
                }
            }
        }
    }
    MPI_Reduce(&localCounter, &globCounter, 1, MPI_INT, MPI_SUM, 0,
               MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    globCounter = (str2.length() - str1.length()) + 2 * globCounter;
    return globCounter;
}

int getCountDiffChSeq(std::string str1, std::string str2) {
    int start = 0;
    int end = str1.length();
    int counter = 0;
    bool uniq;
    for (int i = start; i < end; i++) {
        uniq = true;
        for (int j = 0; j < static_cast<int>(str2.length()); j++) {
            if (str1[i] == str2[j]) {
                uniq = false;
            }
        }
        if (uniq == true) {
            counter++;
        }
    }
    counter = (str2.length() - str1.length()) + 2 * counter;
    return counter;
}
