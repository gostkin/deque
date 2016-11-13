// https://github.com/gostkin/deque
//
// Created by Eugeny Gostkin on 10.11.16.
//

#ifndef DEQUE_H
#define DEQUE_H

#include <cstdlib>
#include <iostream>
#include <iterator>

#define BaseDequeIteratorType DequeIterator<DType, category, value_type, difference_type, pointer, reference>

namespace Deque {
    template <typename CopyType>
    void copy(CopyType *to, const CopyType *from, size_t number) {
        for (size_t i = 0; i < number; ++i)
            to[i] = from[i];
    }

    template <typename DType, typename category, typename value_type, typename difference_type, typename pointer, typename reference>
    class DequeIterator;

    template <typename T>
    class Deque {
    private:
        T *data_;
        size_t max_size_;
        size_t l_pointer_;
        size_t r_pointer_;

        enum class ReallocationType {
            RT_DECREASE,
            RT_INCREASE,
            RT_NONE,
            RT_STAY
        };

        void printReallocationType(ReallocationType type) {
            if (type == ReallocationType::RT_INCREASE) {
                std::cout << "RT_INCREASE\n";
            } else if (type == ReallocationType::RT_DECREASE) {
                std::cout << "RT_DECREASE\n";
            } else if (type == ReallocationType::RT_STAY) {
                std::cout << "RT_STAY\n";
            } else if (type == ReallocationType::RT_NONE) {
                std::cout << "RT_NONE\n";
            }
        }

        ReallocationType needReallocation() const {
            if (4 * size() <= max_size_)
                return ReallocationType::RT_DECREASE;
            if (l_pointer_ == static_cast<size_t>(-1) || r_pointer_ == max_size_ - 1) {
                if (size() < max_size_ / 2)
                    return ReallocationType::RT_STAY;
                else
                    return ReallocationType::RT_INCREASE;
            }

            return ReallocationType::RT_NONE;
        }

        void reallocate(ReallocationType type) {
            if (type == ReallocationType::RT_NONE)
                return;

            if (max_size_ == 2 && type == ReallocationType::RT_DECREASE)
                return;

            size_t size_ = size();
            T *copy_ = new T[size_];
            copy(copy_, data_ + l_pointer_ + 1, size_);

            delete[] data_;

            if (type == ReallocationType::RT_INCREASE)
                max_size_ *= 2;
            if (type == ReallocationType::RT_DECREASE)
                max_size_ /= 2;

            data_ = new T[max_size_];

            copy(data_ + max_size_ / 4, copy_, size_);

            l_pointer_ = max_size_ / 4 - 1;
            r_pointer_ = l_pointer_ + size_;

            delete[] copy_;
        }

    public:
        enum class Errors {
            DE_EMPTY,
            DE_OUT_OF_RANGE
        };

        typedef DequeIterator <Deque <T>, std::random_access_iterator_tag, T, long long, T *, T &> iterator;
        typedef DequeIterator <const Deque <T>, std::random_access_iterator_tag, T, long long, const T *, const T &> const_iterator;
        typedef std::reverse_iterator <iterator> reverse_iterator;
        typedef std::reverse_iterator <const_iterator> const_reverse_iterator;

        Deque() : max_size_(2), l_pointer_(0), r_pointer_(0) {
            data_ = new T[2];
        }

        Deque(const Deque <T> &old) : max_size_(old.max_size_), l_pointer_(old.l_pointer_), r_pointer_(old.r_pointer_) {
            data_ = new T[max_size_];
            copy(data_, old.data_, max_size_);
        }

        ~Deque() {
            delete[] data_;
        }

        inline size_t size() const {
            return r_pointer_ - l_pointer_;
        }

        bool empty() const {
            return size() == 0;
        }

        void push_back(T element) {
            reallocate(needReallocation());

            data_[++r_pointer_] = element;
        }

        void push_front(T element) {
            reallocate(needReallocation());

            data_[l_pointer_--] = element;
        }

        void pop_front() {
            if (empty())
                throw Errors::DE_EMPTY;

            ++l_pointer_;
            reallocate(needReallocation());
        }

        void pop_back() {
            if (empty())
                throw Errors::DE_EMPTY;

            --r_pointer_;
            reallocate(needReallocation());
        }

        T &front() {
            return operator[](0);
        }

        T front() const {
            return operator[](0);
        }

        T &back() {
            return operator[](size() - 1);
        }

        T back() const {
            return operator[](size() - 1);
        }

        const T &operator[](size_t index) const {
            if (l_pointer_ + index + 1 > r_pointer_)
                throw Errors::DE_OUT_OF_RANGE;

            return data_[l_pointer_ + index + 1];
        }

        T &operator[](size_t index) {
            if (l_pointer_ + index + 1 > r_pointer_)
                throw Errors::DE_OUT_OF_RANGE;

            return data_[l_pointer_ + index + 1];
        }

        iterator begin() {
            return iterator(0, this);
        }

        const_iterator cbegin() const {
            return const_iterator(0, this);
        }

        const_iterator begin() const {
            return cbegin();
        }

        iterator end() {
            return iterator(size(), this);
        }

        const_iterator cend() const {
            return const_iterator(size(), this);
        }

        const_iterator end() const {
            return cend();
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(cend());
        }

        const_reverse_iterator rbegin() const {
            return crbegin();
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator crend() const {
            return const_reverse_iterator(cbegin());
        }

        const_reverse_iterator rend() const {
            return crend();
        }

        void print() {
            for (size_t i = 0; i < max_size_; ++i) {
                std::cout << data_[i] << ' ';
            }
            std::cout << std::endl;

            std::cout << "SIZE: " << size() << " MAX_SIZE: " << max_size_ << std::endl;
        }
    };

    template <typename DType, typename category, typename value_type, typename difference_type, typename pointer, typename reference>
    class DequeIterator : public std::iterator <category, value_type, difference_type, pointer, reference> {
    private:
        long long pointer_;
        DType *deque_;

    public:
        DequeIterator(size_t place, DType *d) : pointer_(static_cast<long long>(place)) {
            deque_ = d;
        }

        DequeIterator(const BaseDequeIteratorType &iter) :
                pointer_(iter.pointer_), deque_(iter.deque_) {}

        BaseDequeIteratorType &operator=(BaseDequeIteratorType right) {
            pointer_ = right.pointer_;
            deque_ = right.deque_;

            return *this;
        }

        BaseDequeIteratorType &operator+=(long long right) {
            pointer_ += right;
            return *this;
        }

        BaseDequeIteratorType &operator-=(long long right) {
            pointer_ += -right;
            return *this;
        }

        BaseDequeIteratorType operator+(long long right) const {
            BaseDequeIteratorType temp(*this);

            return temp += right;
        }

        BaseDequeIteratorType operator-(long long right) const {
            BaseDequeIteratorType temp(*this);

            return temp -= right;
        }

        difference_type operator-(BaseDequeIteratorType right) const {
            difference_type answer = static_cast<difference_type>(pointer_) - right.pointer_;

            return answer;
        }

        BaseDequeIteratorType &operator++() {
            return operator+=(1);
        }

        BaseDequeIteratorType operator++(int) {
            BaseDequeIteratorType temp(*this);
            operator+=(1);

            return temp;
        }

        BaseDequeIteratorType &operator--() {
            return operator-=(1);
        }

        BaseDequeIteratorType operator--(int) {
            BaseDequeIteratorType temp(*this);
            operator-=(1);

            return temp;
        }

        bool operator==(BaseDequeIteratorType right) const {
            return pointer_ == right.pointer_ && deque_ == right.deque_;
        }

        bool operator!=(BaseDequeIteratorType right) const {
            return !operator==(right);
        }

        bool operator<(BaseDequeIteratorType right) const {
            return pointer_ < right.pointer_ && deque_ == right.deque_;
        }

        bool operator>=(BaseDequeIteratorType right) const {
            return !operator<(right);
        }

        bool operator>(BaseDequeIteratorType right) const {
            return right < *this;
        }

        bool operator<=(BaseDequeIteratorType right) const {
            return !operator>(right);
        }

        reference operator*() {
            return (*deque_)[pointer_];
        }

        pointer operator->() {
            return &(*deque_)[pointer_];
        }

        reference operator[](long long index) {
            return &(*deque_)[pointer_ + index];
        }

        long long getPointer() const {
            return pointer_;
        }
    };

    template <typename DType, typename category, typename value_type, typename difference_type, typename pointer, typename reference>
    BaseDequeIteratorType operator+(long long left, BaseDequeIteratorType right) {
        return right + left;
    }
}

#endif //DEQUE_H
