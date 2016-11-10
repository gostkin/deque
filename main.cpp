//
// Created by Eugeny Gostkin on 10.11.16.
//

#include <iostream>
#include <vector>

#include "deque.h"

template <typename T>
void pp(Deque<T> &d){
    d[0] = 1;
    for (int i = 0; i < d.size(); ++i)
        std::cout << d[i] << std::endl;
}
int main() {
    Deque<int> d;

    //d.print();
    d.push_back(2);
    //d.print();
    d.push_back(3);
    d.push_front(1);
    //d.print();
    d.push_back(5);
    //d.print();
    d.pop_back();
    //d.print();
    d.pop_front();
    //d.print();
    //pp(d);

    //std::cout << "LEEL\n";
    Deque<int> kek(d);
    //pp(kek);
    //std::cout << d.size() << std::endl;
    //std::cout << d.size() << std::endl;
    //pp(kek);

    //kek.print();
    Deque<int>::const_iterator it = kek.cbegin(), it1 = kek.cend();
    std::cout << kek.size() << std::endl;
    //it.print();
    it1.print();
    //it.print();
    //it.print();
    //std::cout << std::endl;
    //for (; it != kek.cend(); ++it) {
    //    std::cout << *it << std::endl;
    //}
    //std::vector<int> ksek({0, 2});
    //std::cout << ksek.end() - ksek.begin() << std::endl;
    std::cout << it - it1 << '\n';

    return 0;
}