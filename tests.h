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

const double o1_time = 5 * 1e-06;
/* tested on:
 * Intel Core i5-4200U 1.6GHz
 * 8GB RAM
 * AMD Radeon R7 M265 2GB
 *
 * average O(1) time = 1e-06, so we can take constant = 5 and get O(1) time = constant * 1e-06
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

class Check : public ::testing::Test {
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

TEST_F(Check, CheckPushBack) {
    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->push_back(rand() % module);
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_EQ(dq->size(), numberOfElements);
    ASSERT_LE(time / numberOfElements, o1_time);
}


TEST_F(Check, CheckPushFront) {
    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->push_front(rand() % module);
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_EQ(dq->size(), numberOfElements);
    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(Check, CheckPopFront) {
    fill(dq);
    t_time = getTime();

    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->pop_front();
        time += getTime() - t_time;
        t_time = getTime();
    }

    std::cout << dq->empty() << std::endl;
    ASSERT_FALSE(!dq->empty());
    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(Check, CheckPopBack) {
    fill(dq);
    t_time = getTime();

    for (size_t i = 0; i < numberOfElements; ++i) {
        dq->pop_back();
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_FALSE(!dq->empty());
    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(Check, ShuffledOperations) {
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

TEST_F(Check, OperatorSquareBracesNoChange) {
    fill(dq);

    t_time = getTime();
    int value = 0;

    for (size_t i = 0; i < numberOfElements; ++i) {
        value = (*dq)[rand() % numberOfElements];
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(Check, OperatorSquareBraces) {
    fill(dq);

    t_time = getTime();

    for (size_t i = 0; i < numberOfElements; ++i) {
        (*dq)[rand() % numberOfElements] = rand() % module;
        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

TEST_F(Check, PushAndPop) {
    fill(dq);

    t_time = getTime();

    for (size_t i = 0; i < 8 * numberOfElements; ++i) {
        switch (i % 8) {
            case 1:
                for (size_t j = 0; j < 4; ++j)
                    dq->push_front(rand() % module);
                break;
            case 2:
                for (size_t j = 0; j < 4; ++j)
                    dq->pop_back();
                break;
            case 3:
                for (size_t j = 0; j < 4; ++j)
                    dq->pop_front();
                break;
            default:
                for (size_t j = 0; j < 4; ++j)
                    dq->push_back(rand() % module);
                break;
        }

        time += getTime() - t_time;
        t_time = getTime();
    }

    ASSERT_LE(time / numberOfElements, o1_time);
}

template <typename Iterator>
void testLoop(Iterator begin, Iterator end, double &time) {
    time = 0;
    double t_time = getTime();

    for (Iterator it = begin; it != end; ++it) {
        *it;
        time += getTime() - t_time;
        t_time = getTime();
    }
}

TEST_F(Check, IteratorsLoop) {
    fill(dq);

    double time = 0;
    testLoop(dq->begin(), dq->end(), time);
    ASSERT_LE(time / (dq->end() - dq->begin()), o1_time);

    time = 0;
    testLoop(dq->cbegin(), dq->cend(), time);
    ASSERT_LE(time / (dq->cend() - dq->cbegin()), o1_time);

    time = 0;
    testLoop(dq->rbegin(), dq->rend(), time);
    ASSERT_LE(time / (dq->rend() - dq->rbegin()), o1_time);

    time = 0;
    testLoop(dq->rcbegin(), dq->rcend(), time);
    ASSERT_LE(time / (dq->rcend() - dq->rcbegin()), o1_time);
}

#endif //DEQUE_TESTS_H
