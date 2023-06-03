#include <cstddef>
#include <cstdlib>
#include <limits>

#ifndef array_hpp
#define array_hpp

namespace my_vector 
{
    
    template <typename T>
    class Array
    {    
    public:
        std::size_t allocated_size {};
        std::size_t size {};
        const static size_t offset = 4; // first 4 bytes are for storing size and allocated_size

        T *data() 
        {
            void *tmp = reinterpret_cast<char *>(this);
            return static_cast<T *>(tmp);
        }

        static Array<T> *allocate(size_t capacity) noexcept;
        static Array<T> *realloc(Array<T> *data, size_t capacity) noexcept;
        static void deallocate(Array<T> *data);        
        static unsigned calculate_block_size(size_t &capacity);
        static unsigned max_size() {return std::numeric_limits<T>::max() / sizeof(T);}
        static void copy(const T *src_beg, const T *src_end, T *dest_beg);

        typedef T* iterator;
        
        iterator begin() {return data() + offset;} 
        iterator end() {return data() + size + offset;}       
    };
    
}

#include "array_impl.hpp"

#endif
