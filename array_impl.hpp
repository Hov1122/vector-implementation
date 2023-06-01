namespace best_vector 
{	
	template <typename T>
    unsigned Array<T>::calculate_block_size(size_t &capacity)
    {
        unsigned bytes = sizeof(T) * capacity;
        unsigned more_bytes = nextPowerOfTwo(bytes);
 
        bytes += (more_bytes - bytes) / 2;
 
        capacity = bytes / sizeof(T);
        return bytes;
    }
    
    template <typename T>
    Array<T> *Array<T>::allocate(size_t capacity) noexcept
    {
        // assumes the array will grow in future
        
        // ToDo check for overflow cases

        size_t alloc_size = calculate_block_size(capacity) + offset * sizeof(T);

        Array<T> *head = static_cast<Array<T> *>(::malloc(alloc_size));

        head->size = 0;
        head->allocated_size = capacity;
        
        return head;

    }
    
    template <typename T>
    void Array<T>::deallocate(Array<T> *data)
    {
        free(data);
    }

}
