#include <iostream>
#include "vector.hpp"

int main() 
{
    best_vector::Vector<int> v1{7, 4, 5, 6};
    best_vector::Vector<int> v2{3, 5};
    
    int a = 3;
    v1.print();
    v1.push_back(a);
    v1.print();

    v1.push_back(8);
    // v1.print();

    // v1.pop_back();

    int b = 5;
    v1.push_back(b);
    // v1.print();

    best_vector::Vector<int> v2 = v1;
    v1.push_back(14);
    // v2.print();
    // v1.print();
    v2 = v1;
    // v2.print();

    // std::cout << (v1 == v2) << std::endl;
    v1[0] = 4;
    v1.print();

    v1.insert(v1.begin() + 1, 3, 24);
    // v1.insert(v1.begin(), 23);
    // v1.insert(v1.begin(), 23);
    v1.print();

    // std::cout << v1[0] << " " <<  v1[1] << std::endl;
    // std::cout << v2[0] << " " <<  v2[1] << std::endl;

    // best_vector::Vector<int> v3(3);
    // v3.push_back(5);
    // v3.print();

    // best_vector::Vector<int> v4 = best_vector::Vector<int>(5, 3);
    // v4.print();

    // v2.print();
    // v2.erase(2, 2);
    // v2.erase(v2.begin() + 2);

    // v2.insert(v2.begin() + 2, 25);
    // v2.print();

    // v2.clear();
    // v2.print();
    // v2.push_back(4);
    // v2.print();

    // for (auto i : v4)
    // {
    //     std::cout << i << std::endl;
    // }

    // v1 = v2 = v4;
    // v1.print();
    
    return 0;
}