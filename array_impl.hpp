namespace best_vector 
{	
	template <typename T>
    unsigned Array<T>::calculate_block_size(size_t &capacity)
    {
        unsigned bytes = sizeof(T) * capacity;
        if (int(bytes) < 0)
            bytes = unsigned(std::numeric_limits<size_t>::max());
        unsigned more_bytes = nextPowerOfTwo(bytes);
        if (int(more_bytes) < 0)
        {
            //catches morebytes == 2gb
            more_bytes = unsigned(std::numeric_limits<size_t>::max());
            bytes += (more_bytes - bytes) / 2;
        }
        else
        {
            bytes = more_bytes;
        }
 
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
        if (head)
        {
            head->size = 0;
            head->allocated_size = capacity;
        }
        
        return head;

    }
    
    template <typename T>
    void Array<T>::deallocate(Array<T> *data)
    {
        free(data);
    }

}
