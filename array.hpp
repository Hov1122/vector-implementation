#include <cstddef>
#include <cstdlib>

#ifndef array_h
#define array_h

namespace best_vector 
{
    unsigned nextPowerOfTwo(unsigned v)
    {   
        if (v == 0) return 1;
            
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;

        return ++v; 
    }
    
    
    template <typename T>
    class Array
    {    
    public:
        std::size_t allocated_size {};
        std::size_t size {};
        inline static size_t offset = 4; // first 4 bytes are for storing size and allocated_size

        T *data() {
            void *tmp = reinterpret_cast<char *>(this);
            return static_cast<T *>(tmp);
        }

        static Array *allocate(size_t capacity) noexcept;
        static void deallocate(Array<T> *data);        
        static unsigned calculate_block_size(size_t &capacity);

        typedef T* iterator;
        
        iterator begin() {return data() + offset;} 
        iterator end() {return data() + size + offset;}       
    };
    
}

#include "array_impl.hpp"

#endif
