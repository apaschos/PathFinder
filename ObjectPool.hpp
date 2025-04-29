#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

//#define MOCK_POOL

#ifndef MOCK_POOL
template<typename T>
class ObjectPool {
public:
	ObjectPool(size_t size)
        : pool_(std::make_unique<T[]>(size))
        , capacity_(size)
    {
        free_list_.reserve(size);
		for (size_t i = 0; i < size; ++i)
        {
			free_list_.push_back(&pool_[i]);
		}
	}

    T* acquire() {
        if (free_list_.empty())
        {
            throw std::runtime_error("No objects available in pool");
        }

        T* obj = free_list_.back();
        free_list_.pop_back();
        return obj;
    }

    // Release an object back to the pool.
    void release(T* obj)
    {
        free_list_.push_back(obj);
    }

    void releaseAll()
    {
        free_list_.clear();
        for (size_t i = 0; i < capacity_; ++i)
        {
            free_list_.push_back(&pool_[i]);
        }
    }

    // Get current number of available objects
    size_t available() const
    {
        return free_list_.size();
    }

    // Get total pool size
    size_t size() const
    {
        return capacity_;
    }

	private:
        std::unique_ptr<T[]> pool_;  // Single allocation
        std::vector<T*> free_list_;
        size_t capacity_;
	};
#else
template<typename T>
class ObjectPool {
public:
	ObjectPool() {}
	T* acquire() { return new T(); }
	void releaseAll() {}
};
#endif
