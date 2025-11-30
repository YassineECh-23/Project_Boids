//
// Created by Yassine on 30/11/2025.
//
#pragma once
#ifndef PROJECT_BOIDS_DYNAMICARRAY_H
#define PROJECT_BOIDS_DYNAMICARRAY_H
#include <cstddef>

namespace bd {
    template<typename T>
    class DynamicArray {
           private:
        T* data;
        size_t capacity;
        size_t length;
        void resize(std::size_t new_capacity);

           public:
        DynamicArray();
        ~DynamicArray();

    };
}

#endif //PROJECT_BOIDS_DYNAMICARRAY_H