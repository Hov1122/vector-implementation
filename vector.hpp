#include "array.hpp"

#ifndef vector_h
#define vector_h

namespace best_vector {
	
	template <typename T>
	class Vector {
		using Data = Array<T>;
       	Data *d;
        
        void free_data(Data *);

    public:

        template<typename Val>
        class Iterator 
        {
        public:
            using difference_type = typename std::iterator<std::random_access_iterator_tag, Val>::difference_type;
            using iterator_category = std::random_access_iterator_tag;
            using value_type = Val;
            using pointer = Val *;
            using const_pointer = const Val *;
            using reference = Val &; 
            using const_reference = const Val &; 

            Iterator() : _ptr(nullptr) {}
            Iterator(Val *rhs) : _ptr(rhs) {}
            Iterator(const Iterator<Val> &rhs) : _ptr(rhs._ptr) {}
            // operator Iterator<const Val>()  {return Iterator<const Val>(this->_ptr);}

            /* inline Iterator& operator=(Type* rhs) {_ptr = rhs; return *this;} */
            /* inline Iterator& operator=(const Iterator &rhs) {_ptr = rhs._ptr; return *this;} */
            inline Iterator<Val> &operator+=(difference_type rhs) {_ptr += rhs; return *this;}
            inline Iterator<Val> &operator-=(difference_type rhs) {_ptr -= rhs; return *this;}
            inline Val &operator*() const {return *_ptr;}
            inline Val *operator->() const {return _ptr;}
            inline Val &operator[](difference_type rhs) const {return _ptr[rhs];}
            
            inline Iterator<Val> &operator++() {++_ptr; return *this;}
            inline Iterator<Val> &operator--() {--_ptr; return *this;}
            inline Iterator<Val> operator++(int) {Iterator<Val> tmp(*this); ++_ptr; return tmp;}
            inline Iterator<Val> operator--(int) {Iterator<Val> tmp(*this); --_ptr; return tmp;}
            /* inline Iterator operator+(const Iterator& rhs) {return Iterator(_ptr+rhs.ptr);} */
            inline difference_type operator-(const Iterator& rhs) const {return _ptr-rhs.ptr;}
            inline Iterator<Val> operator+(difference_type rhs) const {return Iterator<Val>(_ptr+rhs);}
            inline Iterator<Val> operator-(difference_type rhs) const {return Iterator<Val>(_ptr-rhs);}
            friend inline Iterator<Val> operator+(difference_type lhs, const Iterator<Val> &rhs) 
            {
                return Iterator(lhs+rhs._ptr);
            }
            friend inline Iterator<Val> operator-(difference_type lhs, const Iterator<Val> &rhs)
            {
                return Iterator(lhs-rhs._ptr);
            }
            
            inline bool operator==(const Iterator<Val> &rhs) const {return _ptr == rhs._ptr;}
            inline bool operator!=(const Iterator<Val> &rhs) const {return _ptr != rhs._ptr;}
            inline bool operator>(const Iterator<Val> &rhs) const {return _ptr > rhs._ptr;}
            inline bool operator<(const Iterator<Val> &rhs) const {return _ptr < rhs._ptr;}
            inline bool operator>=(const Iterator<Val> &rhs) const {return _ptr >= rhs._ptr;}
            inline bool operator<=(const Iterator<Val> &rhs) const {return _ptr <= rhs._ptr;}
        private:
            Val *_ptr;
        };

        using iterator = Vector<T>::Iterator<T>;
        using const_iterator = Vector<T>::Iterator<const T>;

        iterator begin() {return iterator(d->begin());} 
        const_iterator begin() const {return const_iterator(d->begin());} 
        const_iterator cbegin() const {return const_iterator(d->begin());} 
        iterator end() {return iterator(d->end());}      
        const_iterator end() const {return const_iterator(d->end());}      
        const_iterator cend() const {return const_iterator(d->end());}      

        void copy(const T *src_beg, const T *src_end, T *dest_beg) ;
        void copy_construct(const T *src_beg, const T *src_end, size_t size) ;
    
        Vector(size_t size);
        Vector(size_t size, T elem);
        Vector(std::initializer_list<T>);
        Vector(const Vector<T> &v);
        ~Vector() {free_data(d);}

        size_t size() const {return d->size;} 
        size_t capacity() const {return d->allocated_size;}
        void print() const;

        void erase(size_t pos); 
        iterator erase(iterator pos); 
        iterator erase(iterator pos, size_t count);
        void erase(size_t pos, size_t count);
        iterator insert(iterator pos, const T val);
        iterator insert(iterator pos, size_t count, const T val);
        void fill(iterator beg, iterator end, T elem);
        void pop_back();
        void push_back(T &elem);
        void push_back(T &&elem);
        bool empty() const {return d->size == 0;}
        void clear();
        void reserve(size_t cap);

        T &back() {return (*this)[size() - 1];}
        T &front() {return (*this)[0];}
        
        T &operator[](size_t i) const;
        Vector<T> &operator=(const Vector<T> &v);
        bool operator==(const Vector<T> &v) const;
	};
	
}

#include "vector_impl.hpp"

#endif
