# include <cstring>
# include "vector.hpp"

namespace best_vector
{

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

    template <typename T>
    Vector<T>::Vector(size_t size) 
    {
        d = Data::allocate(size);
    }

    template<typename T>
    Vector<T>::Vector(size_t size, T elem)
    {
        d = Data::allocate(size);
        d->size = size;
        fill(d->begin(), d->end(), elem);
    }

    template <typename T>
    void Vector<T>::fill(iterator beg, iterator end, T elem)
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

    template <typename T>
    Vector<T>::Vector(std::initializer_list<T> v)
    {   
        copy_construct(v.begin(), v.end(), v.size());
    }

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

    template<typename T>
    void Vector<T>::clear()
    {
        for (auto i = back(); i >= 0; i--)
        {
            pop_back();
        }
    }

    template <typename T>
    void Vector<T>::print() const
    {
        for (size_t i = 0; i < this->size(); i++)
        {
            std::cout << d->begin()[i] << " ";
        }

        std::cout << std::endl;
    }

    template <typename T>
    void Vector<T>::erase(size_t pos)
    {
        if (pos >= size()) throw std::out_of_range("Index is out of range");
        erase(begin() + pos);
    }

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

    template <typename T>
    void Vector<T>::erase(size_t pos, size_t count)
    {
        if (pos + count >= size()) throw std::out_of_range("Index is out of range");
        
        erase(begin() + pos, count);
    }

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

    template <typename T>
    auto Vector<T>::insert(iterator pos, size_t count, const T val) -> iterator
    {
        reserve(size() + count);
        for (size_t i = 0; i < count; i++)
        {
            insert(pos + i, val);
        }

        return pos + count - 1;
    }

    template <typename T>
    Vector<T>::Vector(const Vector<T> &v) 
    {
        copy_construct(v.d->begin(), v.d->end(), v.size());
    }

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

    template <typename T>
    T &Vector<T>::operator[](int i) const
    {
        if ((i >= 0 && abs(i) < size()) || (i < 0 && abs(i) <= size()))
        {
            return i >= 0 ? d->begin()[i] : *(d->end() + i);
        }
           
        throw std::out_of_range ("Index is out of range");
    }  

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
}
