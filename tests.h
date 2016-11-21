/*
 * Copyright [2016] [Eugeny Gostkin]
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
*/

#ifndef DEQUE_TESTS_H
#define DEQUE_TESTS_H

#include <ctime>
#include <deque>
#include <gtest/gtest.h>

#include "deque.h"

/* tested on:
 * Intel Core i5-4200U 1.6GHz @ 2.6 GHz
 * 8GB RAM
 * AMD Radeon R7 M265 2GB
 *
 * average O(1) time = 1e-06 sec, so we can take constant = 5 and get O(1) time = constant * 1e-06
*/

namespace DequeTesting {
    const size_t numberOfElements = 100001;
    const int module = 1000000;

    const double o1_time = 5 * 1e-06;

    inline double getTime() {
        return static_cast<double>(clock()) / CLOCKS_PER_SEC;
    }

    void getNewTime(double &time, double &t_time) {
        time += getTime() - t_time;
        t_time = getTime();
    }

    template <typename DequeType>
    void fill(DequeType *dq) {
        for (size_t i = 0; i < numberOfElements; ++i) {
            dq->push_back(rand() % module);
        }
    }

    class Check : public ::testing::Test {
    public:
        Deque::Deque <int> *dq;

        double time = 0, t_time = 0;

        // time - summary time
        // t_time - last time value

    protected:
        void SetUp() {
            dq = new Deque::Deque <int>;
            time = 0;
            t_time = getTime();
        }

        void TearDown() {
            delete dq;
        }
    };

    class Compare : public ::testing::Test {
    public:
        Deque::Deque <int> *dq;
        std::deque<int> *dq_std;

        double time, t_time;

        // time - summary time
        // t_time - last time value

    protected:
        void SetUp() {
            dq = new Deque::Deque<int>;
            dq_std = new std::deque<int>;

            time = 0;
            t_time = getTime();
        }

        void TearDown() {
            delete dq;
            delete dq_std;
        }
    };

    TEST_F(Check, CheckPushBack) {
        for (size_t i = 0; i < numberOfElements; ++i) {
            dq->push_back(rand() % module);
            getNewTime(time, t_time);
        }

        ASSERT_EQ(dq->size(), numberOfElements);
        ASSERT_LE(time / numberOfElements, o1_time);
    }


    TEST_F(Check, CheckPushFront) {
        for (size_t i = 0; i < numberOfElements; ++i) {
            dq->push_front(rand() % module);
            getNewTime(time, t_time);
        }

        ASSERT_EQ(dq->size(), numberOfElements);
        ASSERT_LE(time / numberOfElements, o1_time);
    }

    TEST_F(Check, CheckPopFront) {
        fill(dq);
        t_time = getTime();

        for (size_t i = 0; i < numberOfElements; ++i) {
            dq->pop_front();
            getNewTime(time, t_time);
        }

        ASSERT_FALSE(!dq->empty());
        ASSERT_LE(time / numberOfElements, o1_time);
    }

    TEST_F(Check, CheckPopBack) {
        fill(dq);
        t_time = getTime();

        for (size_t i = 0; i < numberOfElements; ++i) {
            dq->pop_back();
            getNewTime(time, t_time);
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
            getNewTime(time, t_time);
        }

        ASSERT_LE(time / ((numberOfElements - 1) * 2 + 1), o1_time);
    }

    TEST_F(Check, OperatorSquareBracesNoChange) {
        fill(dq);

        t_time = getTime();
        int value = 0;

        for (size_t i = 0; i < numberOfElements; ++i) {
            value = (*dq)[rand() % numberOfElements];
            getNewTime(time, t_time);
        }

        ASSERT_LE(time / numberOfElements, o1_time);
    }

    TEST_F(Check, OperatorSquareBraces) {
        fill(dq);

        t_time = getTime();

        for (size_t i = 0; i < numberOfElements; ++i) {
            (*dq)[rand() % numberOfElements] = rand() % module;
            getNewTime(time, t_time);
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

            getNewTime(time, t_time);
        }

        ASSERT_LE(time / numberOfElements, o1_time);
    }

    template <typename Iterator>
    void testLoop(Iterator begin, Iterator end, double &time) {
        time = 0;
        double t_time = getTime();

        for (Iterator it = begin; it != end; ++it) {
            *it;
            getNewTime(time, t_time);
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
        testLoop(dq->crbegin(), dq->crend(), time);
        ASSERT_LE(time / (dq->crend() - dq->crbegin()), o1_time);
    }

    TEST_F(Compare, StandardOperations) {
        for (size_t i = 0; i < numberOfElements; ++i) {
            int k = rand() % module - module / 2;
            dq->push_back(k);
            dq_std->push_back(k);
            ASSERT_EQ(dq->size(), dq_std->size());
            ASSERT_EQ(dq->front(), dq_std->front());
            ASSERT_EQ(dq->back(), dq_std->back());
        }
        for (size_t i = 0; i < numberOfElements; ++i) {
            int k = rand() % module - module / 2;
            dq->push_front(k);
            dq_std->push_front(k);
            ASSERT_EQ(dq->size(), dq_std->size());
            ASSERT_EQ(dq->front(), dq_std->front());
            ASSERT_EQ(dq->back(), dq_std->back());
        }
        const Deque::Deque<int> *cd = new Deque::Deque<int>(*dq);
        int constant = 2 * numberOfElements;
        for (int i = 0; i < numberOfElements / 2; ++i) {
            int k = rand() % module - module / 2;
            int index = rand() % (constant);
            ASSERT_EQ((*dq)[index], (*dq_std)[index]);
            ASSERT_EQ(dq->size(), cd->size());
            ASSERT_EQ(cd->front(), dq_std->front());
            ASSERT_EQ(cd->back(), dq_std->back());
            (*dq)[index] = k;
            (*dq_std)[index] = k;
            ASSERT_EQ((*dq)[index], (*dq_std)[index]);
            ASSERT_EQ(dq->front(), dq_std->front());
            ASSERT_EQ(dq->back(), dq_std->back());
        }
        delete cd;
        for (int i = 0; i < constant; ++i) {
            ASSERT_EQ(dq->front(), dq_std->front());
            ASSERT_EQ(dq->back(), dq_std->back());
            if (rand() % 2 == 0) {
                dq->pop_back();
                dq_std->pop_back();
            } else {
                dq->pop_front();
                dq_std->pop_front();
            }

            ASSERT_EQ(dq->size(), dq_std->size());
        }
        ASSERT_EQ(dq->empty(), dq_std->empty());
    }

    template <typename Iter1, typename Iter2>
    void testIters(Iter1 bdq, Iter1 edq, Iter2 sbdq, Iter2 sedq) {
        ASSERT_EQ(bdq, bdq);
        ASSERT_EQ(*bdq, *sbdq);
        for (int i = 0; i < numberOfElements / 2; ++i) {
            int k = rand() % (2 * static_cast<int>(numberOfElements) - 20);
            if (k == 0)
                k = 1;
            ASSERT_EQ(*(bdq + k), *(sbdq + k));
            ASSERT_EQ(*(edq - k), *(sedq - k));
            ASSERT_EQ(*(k + bdq), *(sbdq + k));
            ASSERT_EQ((edq - k) - bdq, (sedq - k) - sbdq);
            ASSERT_LE(bdq, edq - k);
            ASSERT_EQ(static_cast<int>(bdq.operator[](k)), static_cast<int>(sbdq.operator[](k)));
        }
    }

    template <typename Iter1, typename Iter2>
    void testNonConstIters(Iter1 bdq, Iter1 edq, Iter2 sbdq, Iter2 sedq) {
        ASSERT_EQ(bdq, bdq);
        ASSERT_EQ(*bdq, *sbdq);
        for (int i = 0; i < numberOfElements / 2; ++i) {
            int k = rand() % (2 * static_cast<int>(numberOfElements) - 20);
            int l = rand() % module;
            if (k == 0)
                k = 1;
            *(bdq + k) = l;
            *(sbdq + k) = l;
            ASSERT_EQ(*(bdq + k), *(sbdq + k));
            ASSERT_EQ(*(edq - k), *(sedq - k));
            ASSERT_EQ(*(k + bdq), *(sbdq + k));
            ASSERT_EQ((edq - k) - bdq, (sedq - k) - sbdq);
            ASSERT_LE(bdq, edq - k);
            ASSERT_EQ(static_cast<int>(bdq.operator[](k)), static_cast<int>(sbdq.operator[](k)));
            ++sbdq;
            ++bdq;
            bdq++;
            sbdq++;
            bdq += 2;
            sbdq += 2;
            ASSERT_EQ(*(bdq + k), *(sbdq + k));
            ASSERT_EQ(*(edq - k), *(sedq - k));
            ASSERT_EQ(*(k + bdq), *(sbdq + k));
            ASSERT_EQ((edq - k) - bdq, (sedq - k) - sbdq);
            ASSERT_LE(bdq, edq - k);
            ASSERT_EQ(static_cast<int>(bdq.operator[](k)), static_cast<int>(sbdq.operator[](k)));
            bdq -= 2;
            sbdq -= 2;
            --bdq;
            --bdq;
            --sbdq;
            --sbdq;
        }
    }

    TEST_F(Compare, Iterators) {
        for (size_t i = 0; i < numberOfElements; ++i) {
            int k = rand() % module - module / 2;
            dq->push_back(k);
            dq_std->push_back(k);
            ASSERT_EQ(dq->size(), dq_std->size());
            ASSERT_EQ(dq->front(), dq_std->front());
            ASSERT_EQ(dq->back(), dq_std->back());
        }
        for (size_t i = 0; i < numberOfElements; ++i) {
            int k = rand() % module - module / 2;
            dq->push_front(k);
            dq_std->push_front(k);
            ASSERT_EQ(dq->size(), dq_std->size());
            ASSERT_EQ(dq->front(), dq_std->front());
            ASSERT_EQ(dq->back(), dq_std->back());
        }

        testIters(dq->begin(), dq->end(), dq_std->begin(), dq_std->end());
        testIters(dq->cbegin(), dq->cend(), dq_std->cbegin(), dq_std->cend());
        testIters(dq->rbegin(), dq->rend(), dq_std->rbegin(), dq_std->rend());
        testIters(dq->crbegin(), dq->crend(), dq_std->crbegin(), dq_std->crend());

        testNonConstIters(dq->begin(), dq->end(), dq_std->begin(), dq_std->end());
        testNonConstIters(dq->rbegin(), dq->rend(), dq_std->rbegin(), dq_std->rend());
    }
}



#endif //DEQUE_TESTS_H
