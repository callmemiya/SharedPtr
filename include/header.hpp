// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <algorithm>
#include <utility>
#include <atomic>

template <typename T>
class SharedPtr {
public:
    SharedPtr(){
        ptr = nullptr;
        copy = nullptr;
    }
    explicit SharedPtr(T* pt){
        ptr = pt;
        copy = new std::atomic_uint();
        *copy = 1;
    }
    SharedPtr(const SharedPtr& r){
        ptr = r.ptr;
        copy = r.copy;
        (*copy)++;
    }
    SharedPtr(SharedPtr&& r){
        ptr = r.ptr;
        copy = r.copy;
        r.ptr = nullptr;
        r.copy = nullptr;
    }
    ~SharedPtr(){
        if (ptr != nullptr){
            if (*copy == 1){
                delete ptr;
                delete copy;
            } else {
                (*copy)--;
            }
        }
    }
    auto operator=(const SharedPtr& r) -> SharedPtr&{
        if (ptr != r.ptr){
            ptr = r.ptr;
            copy = r.copy;
            (*copy)++;
        }
        return *this;
    }
    auto operator=(SharedPtr&& r) -> SharedPtr&{
        ptr = r.ptr;
        copy = r.copy;
        r.ptr = nullptr;
        r.copy = nullptr;
        return *this;
    }

    // проверяет, указывает ли указатель на объект
    operator bool() const{
        if (ptr == nullptr) return false;
        else
            return true;
    }
    auto operator*() const -> T&{
        return *ptr;
    }
    auto operator->() const -> T*{
        return ptr;
    }
    auto get() -> T*{
        return ptr;
    }
    void reset(){
        if (copy != nullptr){
            (*copy)--;
            if (*copy == 0){
                delete ptr;
                delete copy;
            }
        }
        ptr = nullptr;
        copy = nullptr;
    }
    void reset(T* pt){
        if (copy != nullptr){
            (*copy)--;
            if (*copy == 0){
                delete ptr;
                delete copy;
            }
        }
        ptr = pt;
        copy = new std::atomic_uint();
        *copy = 1;
    }
    void swap(SharedPtr& r){
        std::swap(ptr, r.ptr);
        std::swap(copy, r.copy);
    }
    auto use_count() const -> size_t{
        if (copy != nullptr) return *copy;
        else
            return 0;
    }

private:
    T* ptr;
    std::atomic_uint* copy;
};

#endif // INCLUDE_HEADER_HPP_
