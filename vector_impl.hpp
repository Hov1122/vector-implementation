#ifndef vector_impl_hpp
#define vector_impl_hpp

#ifndef vector_hpp
#error [Error] vector_impl_hpp should only be included from vector.hpp
#endif

# include <cstring>
# include "vector.hpp"

namespace best_vector
{

    // copise memory between src_beg and src_end to dest_end
    template <typename T>
    void Vector<T>::copy(const T *src_beg, const T *src_end, T *dest_beg) 
    {
        if (std::is_trivial<T>::value)
        {
            ::memcpy(static_cast<void *>(dest_beg), static_cast<const void *>(src_beg), (src_end - src_beg ) * sizeof(T));
            return;
        } 

        while (src_beg < src_end) {
            new (dest_beg++) T(*src_beg++);
        }
    }

    // creates vector with size 'size' and initializes all values to the type T's default value
    template <typename T>
    Vector<T>::Vector(size_t size) 
    {
        d = Data::allocate(size);
    }

    // creates vector with elements elem by size 'size'
    template<typename T>
    Vector<T>::Vector(size_t size, T elem)
    {
        d = Data::allocate(size);
        d->size = size;
        fill(elem, d->begin(), d->end());
    }

    // fills vector with value of elem starting from b to e
    template <typename T>
    void Vector<T>::fill(T elem, iterator beg, iterator end)
    {
        while (beg < end) 
        {
            if (!std::is_trivial<T>::value)
            {
                new (&(*beg)) T(elem);
            }
            else 
            {
                *beg = elem;
            }
            beg++;
        }
    }
    
    // fills vector with value of elem starting from index b to e
    template <typename T>
    void Vector<T>::fill(T &elem, size_t b, int e)
    {
        if (e < 0) e += size();
        fill(elem, begin() + b, begin() + e);
    }

    // fills vector with value of elem starting from index b to e
    template <typename T>
    void Vector<T>::fill(T &&elem, size_t b, int e)
    {
        if (e < 0) e += size() + 1;

        fill(elem, begin() + b, begin() + e);
    }

    // allocates size bytes. copies memory between src_beg and src_end to the vector's begining. sets vector size to size 
    // if it can't cant allocate enough memory throws error.  
    template <typename T>
    void Vector<T>::copy_construct(const T *src_beg, const T *src_end, size_t size)
    {
        if (size > 0) 
        {
            d = Data::allocate(size);

            if (d == nullptr) throw "No memory";
        
            copy(src_beg, src_end, d->begin());

            d->size = size;
        }
        else 
        {
            d = nullptr;
            d->size = 0;
        }
    }

    // creates vector from values of v
    template <typename T>
    Vector<T>::Vector(std::initializer_list<T> v)
    {   
        copy_construct(v.begin(), v.end(), v.size());
    }

    // frees the allocated memory
    template <typename T>
    void Vector<T>::free_data(Data *d) 
    {
        T *from = d->begin() - Data::offset;
        T *to = d->end();
        
        if (std::is_trivial<T>::value)
        {
            while (from != to)
                from++->~T();    
        }    
        Data::deallocate(d);
    }

    // deletes the last element of the vector
    template <typename T>
    void Vector<T>::pop_back() 
    {
        if (d->size <= 0) return;
        if (!std::is_trivial<T>::value) 
        {
            d->begin()[d->size - 1].~T();
        }
        d->size -= 1;
    }   

    // appends element at the back of the vector
    template <typename T>
    void Vector<T>::push_back(T &elem) 
    {
        reserve(d->size + 1);
        std::cout << d->size << std::endl;
        if (std::is_trivial<T>::value) 
        {
            d->begin()[d->size++] = elem;
        }
        else 
        {
            new (&d->begin()[d->size - 1]) T(elem);
        }
    } 

    // appends element at the back of the vector
    template <typename T>
    void Vector<T>::push_back(T &&elem) 
    {
        reserve((d->size + 1) * 2);
        std::cout << d->size << std::endl;
        if (std::is_trivial<T>::value) 
        {
                d->begin()[d->size++] = elem;
        }
        else 
        {
            new (&d->begin()[d->size - 1]) T(elem);
        }
    }

    // deletes all elements from vector
    template<typename T>
    void Vector<T>::clear()
    {
        for (auto i = back(); i >= 0; i--)
        {
            pop_back();
        }
    }

    // prints vector in format  [elem_1, elem_2, ... elem_n]. Type T should implement std::ostream
    template <typename T>
    void Vector<T>::print() const
    {
        std::cout << "[";
        for (size_t i = 0; i < size() - 1; i++)
        {
            std::cout << (*this)[i] << ", ";
        }

        std::cout << (*this)[size() - 1] <<  "]" << std::endl;
    }

    // deletes element at index pos
    template <typename T>
    void Vector<T>::erase(size_t pos)
    {
        if (pos >= size()) throw std::out_of_range("Index is out of range");
        erase(begin() + pos);
    }

    // deletes element at pos
    template <typename T>
    auto Vector<T>::erase(iterator pos) -> iterator
    {
        if (pos >= end()) throw std::out_of_range("Index is out of range");

        if (std::is_trivial<T>::value) 
        {
            pos->~T();
        }

        for (iterator start = pos; start != end() - 1; start++)
        {
            *start = *(start + 1);
        }

        d->size--;

        return pos;
    }

    // deletes count number of elements starting of pos
    template <typename T> 
    auto Vector<T>::erase(iterator pos, size_t count) -> iterator
    {

        if (pos + count >= end()) throw std::out_of_range("Index is out of range");

        for (size_t i = 0; i < count; i++)
        {
            erase(pos);
        }

        return pos;
    }

    // deletes count number of elements at index pos
    template <typename T>
    void Vector<T>::erase(size_t pos, size_t count)
    {
        if (pos + count >= size()) throw std::out_of_range("Index is out of range");
        
        erase(begin() + pos, count);
    }

    // inserts val at pos
    template <typename T>
    auto Vector<T>::insert(iterator pos, const T val) -> iterator
    {
        reserve(size() + 1);
        
        d->size++;
        for (iterator start = end() - 1; start != pos - 1; start--)
        {
            *start = *(start - 1);
        }

        if (!std::is_trivial<T>::value) 
        {
            new (&(*(pos))) T(val); 
        }
        else 
        {
            *(pos) = val;
        }
        
        return pos;
    }

    // inserts count number of val-s starting from pos
    template <typename T>
    auto Vector<T>::insert(iterator pos, const T val, size_t count) -> iterator
    {
        reserve(size() + count);
        for (size_t i = 0; i < count; i++)
        {
            insert(pos + i, val);
        }

        return pos + count - 1;
    }

    // inserts count number of val-s starting from pos
    template <typename T>
    void Vector<T>::insert(size_t pos, const T val, size_t count)
    {
        insert(begin() + pos, val, count);
    }

    template <typename T>
    Vector<T>::Vector(const Vector<T> &v) 
    {
        copy_construct(v.d->begin(), v.d->end(), v.size());
    }

    // copies elements of v to this
    template <typename T>
    Vector<T> &Vector<T>::operator=(const Vector<T> &v)
    {
        if (capacity() >= v.capacity())
        {
            copy(v.d->begin(), v.d->end(), d->begin());
            d->size = v.size();
            return *this;
        }

        free_data(d);
        copy_construct(v.d->begin(), v.d->end(), v.size());

        return *this;
    }

    // compares vector elements element by element
    template <typename T>
    bool Vector<T>::operator==(const Vector &v) const
    {
        if (size() != v.size())
            return false;

        for (size_t i = 0; i < size(); i++)
        {
            if ((*this)[i] != (v[i]))
                return false;
        }

        return true;
    }  

    // returns reference to the i-th element of vector. supports negative indexing
    template <typename T>
    T &Vector<T>::operator[](int i) const
    {
        if ((i >= 0 && abs(i) < size()) || (i < 0 && abs(i) <= size()))
        {
            return i >= 0 ? d->begin()[i] : *(d->end() + i);
        }
           
        throw std::out_of_range ("Index is out of range");
    }  

    // allocates no less than cap bytes for vector
    template <typename T>
    void Vector<T>::reserve(size_t cap)
    {
        if (cap <= capacity())
            return;

        Data *new_d = Data::allocate(cap);
        copy(d->begin(), d->end(), new_d->begin());
        size_t s = d->size;
        free_data(d);
        d = new_d;
        d->size = s;
    }

    // sets vector size to s. Fills with the default value of type T if s >= size()
    template <typename T>
    void Vector<T>::resize(size_t s)
    {
        reserve(s);

        if (!std::is_trivial<T>::value && s < size())
        {
            for (size_t i = s; i < size(); i++)
            {
                (*this)[i].~T();
            }
        }
        
        d->size = s;
    }

    // sets vector size to s. Fills with val if s >= size()
    template <typename T>
    void Vector<T>::resize(size_t s, const T &val)
    {
        size_t old_size = size();
        reserve(s);
        d->size = s;
        
        if (!std::is_trivial<T>::value && s < old_size)
        {
            for (size_t i = s; i < old_size; i++)
            {
                (*this)[i].~T();
            }
        }
        else if (s >= old_size)
        {
            for (size_t i = old_size; i < s; i++)
            {
                
                if (std::is_trivial<T>::value)
                    (*this)[i] = val;
                else
                    (*this)[i] = T(val);
            }
        }  
    }

    // returns index of elem if found in vector starting from 'from'. returns -1 if doesn't find.
    template <typename T>
    int Vector<T>::index_of(const T &elem, size_t from) const
    {
        for (size_t i = from; i < size(); i++)
        {
            if ((*this)[i] == elem)
                return i;
        }

        return -1;
    }

    // returns last index of elem if found in vector starting from 'from'. returns -1 if doesn't find.
    template <typename T>
    int Vector<T>::last_index_of(const T &elem, int from) const
    {
        if (from < 0) from += size(); 

        for (int i = from; i >= 0; i--)
        {
            if ((*this)[i] == elem)
                return i;
        }

        return -1;
    }

    // checks if vector contains any element satisfying to any_comp condition
    template <typename T>
    template <typename Functor>
    bool Vector<T>::any(Functor any_comp)
    {
        for (size_t i = 0; i < size(); i++)
        {
            if (any_comp((*this)[i]))
                return true;
        }
        return false;
    }

    // checks if vectors each element satisfying to any_comp condition
    template <typename T>
    template <typename Functor>
    bool Vector<T>::every(Functor every_comp)
    {
        for (size_t i = 0; i < size(); i++)
        {
            if (!every_comp((*this)[i]))
                return false;
        }
        return true;
    }

    // prints vector in format  [elem_1, elem_2, ... elem_n]. Type T should implement std::ostream
    template <typename T>
    std::ostream &operator<<(std::ostream &stream, const Vector<T> &v) 
    {
        stream << "[";
        for (size_t i = 0; i < v.size() - 1; i++)
        {
            stream << v.d->begin()[i] << ", ";
        }

        stream << v.d->begin()[v.size() - 1] <<  "]" << std::endl;

        return stream;
    }
}

#endif
