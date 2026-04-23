
#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace fast {

template<typename T>
class vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    
    void reallocate(size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        
        for (size_t i = 0; i < size_; ++i) {
            new(&new_data[i]) T(std::move(data_[i]));
            data_[i].~T();
        }
        
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
    
public:
    vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    explicit vector(size_t count) : data_(nullptr), size_(0), capacity_(0) {
        reserve(count);
        for (size_t i = 0; i < count; ++i) {
            emplace_back();
        }
    }
    
    vector(size_t count, const T& value) : data_(nullptr), size_(0), capacity_(0) {
        reserve(count);
        for (size_t i = 0; i < count; ++i) {
            push_back(value);
        }
    }
    
    vector(const vector& other) : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.capacity_);
        for (size_t i = 0; i < other.size_; ++i) {
            push_back(other.data_[i]);
        }
    }
    
    vector(vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    ~vector() {
        clear();
        ::operator delete(data_);
    }
    
    vector& operator=(const vector& other) {
        if (this != &other) {
            clear();
            reserve(other.capacity_);
            for (size_t i = 0; i < other.size_; ++i) {
                push_back(other.data_[i]);
            }
        }
        return *this;
    }
    
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data_);
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new(&data_[size_]) T(value);
        ++size_;
    }
    
    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new(&data_[size_]) T(std::move(value));
        ++size_;
    }
    
    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        new(&data_[size_]) T(std::forward<Args>(args)...);
        ++size_;
    }
    
    void pop_back() {
        if (size_ > 0) {
            data_[size_ - 1].~T();
            --size_;
        }
    }
    
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            reallocate(new_capacity);
        }
    }
    
    void resize(size_t new_size) {
        if (new_size < size_) {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
            size_ = new_size;
        } else if (new_size > size_) {
            reserve(new_size);
            for (size_t i = size_; i < new_size; ++i) {
                emplace_back();
            }
        }
    }
    
    void resize(size_t new_size, const T& value) {
        if (new_size < size_) {
            for (size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
            size_ = new_size;
        } else if (new_size > size_) {
            reserve(new_size);
            for (size_t i = size_; i < new_size; ++i) {
                push_back(value);
            }
        }
    }
    
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
    
    void shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_);
        }
    }
    
    T& operator[](size_t index) {
        return data_[index];
    }
    
    const T& operator[](size_t index) const {
        return data_[index];
    }
    
    T& at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("vector::at");
        }
        return data_[index];
    }
    
    const T& at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("vector::at");
        }
        return data_[index];
    }
    
    T& front() {
        return data_[0];
    }
    
    const T& front() const {
        return data_[0];
    }
    
    T& back() {
        return data_[size_ - 1];
    }
    
    const T& back() const {
        return data_[size_ - 1];
    }
    
    T* data() {
        return data_;
    }
    
    const T* data() const {
        return data_;
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    size_t size() const {
        return size_;
    }
    
    size_t capacity() const {
        return capacity_;
    }
    
    T* begin() {
        return data_;
    }
    
    T* end() {
        return data_ + size_;
    }
    
    const T* begin() const {
        return data_;
    }
    
    const T* end() const {
        return data_ + size_;
    }
    
    const T* cbegin() const {
        return data_;
    }
    
    const T* cend() const {
        return data_ + size_;
    }
};

} // namespace fast

#endif // VECTOR_H