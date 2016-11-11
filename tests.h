//
// Created by gosktin on 11.11.16.
//

#ifndef DEQUE_TESTS_H
#define DEQUE_TESTS_H

#include <ctime>
#include <gtest/gtest.h>

#include "deque.h"

const size_t numberOfElements = 100001;
const int module = 1000000;

const double o1_time = 10 * 1e-06;
/* tested on:
 * Intel Core i5-4200U 1.6GHz
 * 8GB RAM
 * AMD Radeon R7 M265 2GB
 *
 * average O(1) time = 1e-06, so we can take constant = 10 and get O(1) time = constant * 1e-06
*/


inline double getTime() {
    return static_cast<double>(clock()) / CLOCKS_PER_SEC;
}

template <typename DataType>
void fill(Deque::Deque<DataType> *dq) {
    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->push_back(rand() % module);
    }
}

class CheckOperationsTime : public ::testing::Test {
public:
    Deque::Deque<int> *dq;

    double time = 0, t_time = 0;

    // time - summary time
    // t_time - last time value
    // f_time - O(1) operation time

protected:
    void SetUp() {
        dq = new Deque::Deque<int>;
    }

    void TearDown() {
        delete dq;
    }

    void SetUpTextCase() {
        time = 0;
        t_time = getTime();
    }
};

TEST_F(CheckOperationsTime, CheckPushBack) {
    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->push_back(rand() % module);
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}


TEST_F(CheckOperationsTime, CheckPushFront) {
    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->push_front(rand() % module);
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(CheckOperationsTime, CheckPopFront) {
    fill(dq);
    t_time = getTime();

    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->pop_front();
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(CheckOperationsTime, CheckPopBack) {
    fill(dq);
    t_time = getTime();

    for (size_t i = 0; i < numberOfElements - 1; ++i) {
        dq->pop_back();
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(CheckOperationsTime, ShuffledOperations) {
    dq->push_back(rand() % module);

    time = o1_time;
    t_time = getTime();

    for (size_t i = 0; i < (numberOfElements - 1) * 2; ++i) {
        int operation = dq->size() > 0 ? rand() % 4 : rand() % 2;
        switch (operation) {
            case 0:
                dq->push_front(rand() % module);
                break;
            case 1:
                dq->push_back(rand() % module);
                break;
            case 3:
                dq->pop_front();
                break;
            case 4:
                dq->pop_back();
                break;
            default:
                dq->push_back(rand() % module);
                break;
        }
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / ((numberOfElements - 1) * 2 + 1), o1_time);
}

TEST_F(CheckOperationsTime, OperatorSquareBracesNoChange) {
    fill(dq);

    time = getTime();
    int value = 0;

    for (size_t i = 0; i < numberOfElements; ++i) {
        value = (*dq)[rand() % numberOfElements];
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(CheckOperationsTime, OperatorSquareBraces) {
    fill(dq);

    time = getTime();

    for (size_t i = 0; i < numberOfElements; ++i) {
        (*dq)[rand() % numberOfElements] = rand() % module;
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

#endif //DEQUE_TESTS_H
