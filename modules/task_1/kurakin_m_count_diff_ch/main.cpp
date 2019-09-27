// Copyright 2019 Kurakin Mikhail
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "./count_diff_ch.h"

TEST(Count_Diff_Ch_MPI, Test_Rand_Str) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
    unsigned int expLen = 4;
    std::string str = getRandomString(4);
    if (rank == 0) {
        EXPECT_EQ(expLen, str.length());
    }
}
TEST(Count_Diff_Ch_MPI, Test_Find_Count_Diff_Ch_Sequential) {
    int rank;
    int expResult = 17;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "Mika12345";
    std::string str2 = "arbuz6789x";
    int countDiff;
    if (rank == 0) {
        countDiff = getCountDiffChSeq(str1, str2);
        EXPECT_EQ(expResult, countDiff);
    }
}
TEST(Count_Diff_Ch_MPI, Test_Find_Count_Diff_Ch_Sequential_Rand) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
    std::string str1, str2;
    if (rank == 0) {
    str1 = getRandomString(200);
    str2 = getRandomString(200);
    }
    if (rank == 0) {
        ASSERT_NO_THROW(getCountDiffChSeq(str1, str2));
    }
}
TEST(Count_Diff_Ch_MPI, Test_Find_Count_Diff_Ch_Parallel) {
    int rank;
    int expResult;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "Mika12349";
    std::string str2 = "arbuz5678x";
    int countDiff = getCountDiffChPar(str1, str2);
    if (rank == 0) {
        expResult = getCountDiffChSeq(str1, str2);
        EXPECT_EQ(expResult, countDiff);
    }
}

TEST(Count_Diff_Ch_MPI, Test_Find_Count_Diff_Ch_Parallel_Rand) {
    int rank;
    int expResult;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(NULL));
    std::string str1, str2;
    str1 = getRandomString(300);
    str2 = getRandomString(300);
    int countDiff = getCountDiffChPar(str1, str2);
    if (rank == 0) {
        expResult = getCountDiffChSeq(str1, str2);
        EXPECT_EQ(expResult, countDiff);
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

