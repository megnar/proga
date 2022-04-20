#define _CRT_SECURE_NO_WARNING

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_set>
#include <cmath>
#include <string>
#include <sstream>
#include <numeric>
#include <list>
#include <thread>
#include <functional>
#include <cstdio>
using namespace std;
class My
{
public:
    virtual ~My()
    {
        std::cout << "Hello from ~My()" << std::endl;
    }
    virtual void func() {
        std::cout << "1";

    }
};

class Derived : public My
{
public:
    ~Derived()
    {
        // Здесь могла бы быть очистка ресурсов
        std::cout << "Hello from ~Derived()" << std::endl;
    }
};
class X : public Derived
{
public:
    ~X()
    {
        // Здесь могла бы быть очистка ресурсов
        std::cout << "Hello from ~x()" << std::endl;
    }
    void func()override{
        std::cout << "2";
    }
};
using namespace std;

template <typename T>
class auto_ptr_{
public:
    auto_ptr_(T* ptr) : ptr_(ptr) {}

    ~auto_ptr_(){
        cout << "destruct";
        delete ptr_;
    }

private:
    T* ptr_;
};

class A{
public:
    A(int x){
        cout << x << endl;
    }
    ~A(){
        cout << "A destroyed" << endl;
    }
};

int main(){


    cout << endl;

    int x = 1;
    auto ptr_  = new A(5);
    auto_ptr<A>* y (ptr_);
    //auto_ptr_<A>* y (ptr_);


}
