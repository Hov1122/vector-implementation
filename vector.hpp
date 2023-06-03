/*
 * Copyright (c) 2023 Hov1122 <<url>>
 * 
 * Created Date: Saturday, June 3rd 2023, 10:55:19 am
 * Author: Hov1122
 * Last Modified: Saturday, 3rd June 2023 11:58:51 am
 * Modified By: Hov1122
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS
 * IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ----------	---	----------------------------------------------------------
 */


#ifndef vector_hpp
#define vector_hpp

#include "array.hpp"

namespace my_vector {
	
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
            // inline Iterator operator+(const Iterator& rhs) {return Iterator(_ptr+rhs._ptr);} 
            inline difference_type operator-(const Iterator& rhs) const {return _ptr-rhs._ptr;}
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
        Vector(Vector<T> &&other) noexcept : d(other.d) {other.d = nullptr;}
        ~Vector() {free_data(d);}

        size_t size() const {return d->size;} 
        size_t capacity() const {return d->allocated_size;}
        void print() const;

        void erase(size_t pos); 
        iterator erase(iterator pos); 
        iterator erase(iterator pos, size_t count);
        void erase(size_t pos, size_t count = 1);
        iterator insert(iterator pos, const T val);
        iterator insert(iterator pos, const T val, size_t count);
        void insert(size_t pos, const T val, size_t count = 1);
        void fill(T elem, iterator beg, iterator end);
        void fill(T &elem, size_t b = 0, int e = -1);
        void fill(T &&elem, size_t b = 0, int e = -1);
        void pop_back();
        void pop_front() {erase(0);}
        void push_back(T &elem);
        void push_back(T &&elem);
        void push_front(T &elem) {insert(begin(), elem);}
        void push_front(T &&elem) {insert(begin(), elem);}
        bool is_empty() const {return d->size == 0;}
        void clear();
        void reserve(size_t cap);
        void resize(size_t count);
        void resize(size_t count, const T &val);
        void shrink_to_fit() {d->capacity = d->size;};
        void swap(Vector<T> &other) noexcept {swap(d, other.d);}
        size_t length() {return d->size();}
        int index_of(const T &elem, size_t from = 0) const;
        int last_index_of(const T &elem, int from = -1) const;
        bool contains(const T &elem) const {return index_of(elem) != -1;};
        T *data() noexcept {return begin();}

        template<typename Functor>
        bool any(Functor any_comp, size_t from = 0);

        template<typename Functor>
        bool every(Functor every_comp, size_t from = 0);

        template<typename Functor>
        iterator find(Functor find_comp, size_t from = 0);

        template<typename Functor>
        int find_index(Functor find_comp, size_t from = 0);

        T &back() {return (*this)[size() - 1];}
        T &front() {return (*this)[0];}
        
        T &operator[](int i) const;
        Vector<T> &operator=(const Vector<T> &v);
        bool operator==(const Vector<T> &v) const;
        bool operator!=(const Vector<T> &v) const {return !(*this == v);};

        template <typename U>
        friend std::ostream &operator<<(std::ostream &stream, const Vector<U> &v);
	};
	
}

#include "vector_impl.hpp"

#endif
