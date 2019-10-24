// Copyright 2019 Streltsov Roman
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cstring>
#include "./frequency_count_letter.h"


TEST(Frequency_Count_Letter, Test_Can_Create_Random_String_Word) {
    int sizeWord = 15;
    char* word = new char[sizeWord];
    ASSERT_NO_THROW(getRandomStr(word, sizeWord));
}

TEST(Frequency_Count_Letter, Test_Negative_Size_String_Word) {
    int sizeWord = -2;
    char* word = new char[10];
    ASSERT_ANY_THROW(getRandomStr(word, sizeWord));
}


TEST(Frequency_Count_Letter, Test_Count_Letter) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    char word[] = "helloitistest";
    char letter = 't';

    int result = getCountParallelFreqLettInWord(word, letter);

    if (proc_rank == 0) {
        int expected = getCountFreqLettInWord(word, letter);
        ASSERT_EQ(expected, result);
    }
}

TEST(Frequency_Count_Letter, Test_Count_With_Missing_Letter) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    char word[] = "helloitistest";
    char letter = 'z';

    int result = getCountParallelFreqLettInWord(word, letter);

    if (proc_rank == 0) {
        int expected = getCountFreqLettInWord(word, letter);
        ASSERT_EQ(expected, result);
    }
}

TEST(Frequency_Count_Letter, Test_Count_Letter_In_Random_Word) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    int sizeWord = 20;
    char letter = 's';
    char* word = new char[sizeWord + 1];

    getRandomStr(word, sizeWord);

    int result = getCountParallelFreqLettInWord(word, letter);

    if (proc_rank == 0) {
        int expected = getCountFreqLettInWord(word, letter);
        ASSERT_EQ(expected, result);
    }
}

TEST(Frequency_Count_Letter, Test_Count_Same_Letters) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    char word[] = "yyyyyyyyyyyyy";
    char letter = 'y';

    int result = getCountParallelFreqLettInWord(word, letter);
    if (proc_rank == 0) {
        int expected = getCountFreqLettInWord(word, letter);
        ASSERT_EQ(expected, result);
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
