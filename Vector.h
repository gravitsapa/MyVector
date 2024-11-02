#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <stdexcept>

#include <vector>

template <class T, class Allocator = std::allocator<T>>
class my_vector {
public:
    class iterator {
        friend 
        class my_vector<T>;
    public:
        iterator() = default;

        iterator(my_vector* obj, size_t ind) : obj_(obj), ind_(ind) {
        }

        iterator operator+(size_t diff) {
            iterator res = iterator(obj_, ind_ + diff);
            res.check_correct();
            return res;
        }

        iterator operator-(size_t diff) {
            iterator res = iterator(obj_, ind_ - diff);
            res.check_correct();
            return res;
        }

        iterator& operator++() {
            ind_++;
            check_correct();
            return *this;
        }
        iterator operator++(int) {
            iterator res = iterator(obj_, ind_++);
            check_correct();
            return res;
        }

        iterator& operator--() {
            ind_--;
            check_correct();
            return *this;
        }
        iterator operator--(int) {
            iterator res = iterator(obj_, ind_--);
            check_correct();
            return res;
        }

        T& operator*() const {
            check_correct();
            return obj_->data_[ind_];
        }
        T* operator->() const {
            check_correct();
            return obj_->data_ + ind_;
        }

        bool operator==(const iterator& anoth) const {
            return obj_ == anoth.obj_ && ind_ == anoth.ind_;
        }

        bool operator!=(const iterator& anoth) const {
            return obj_ != anoth.obj_ || ind_ != anoth.ind_;
        }
        
        bool operator<(const iterator& anoth) const {
            check_obj(anoth);
            return ind_ < anoth.ind_;
        }

        bool operator>(const iterator& anoth) const {
            check_obj(anoth);
            return ind_ > anoth.ind_;
        }

        bool operator<=(const iterator& anoth) const {
            check_obj(anoth);
            return ind_ <= anoth.ind_;
        }

        bool operator>=(const iterator& anoth) const {
            check_obj(anoth);
            return ind_ >= anoth.ind_;
        }


    private:
        void check_obj(const iterator& anoth) const {
            if (obj_ != anoth.obj_) {
                throw std::exception();
            }
        }

        void check_correct() const {
            if (obj_ == nullptr) {
                throw std::exception();
            }
            if (ind_ > obj_->size_) {
                throw std::exception();
            }
        }

        my_vector* obj_ = nullptr;
        size_t ind_ = 0;
    };

    class const_iterator {
        friend 
        class my_vector<T>;
    public:
        const_iterator() = default;

        const_iterator(const my_vector* obj, size_t ind) : obj_(obj), ind_(ind) {
        }

        const_iterator operator+(size_t diff) {
            const_iterator res = const_iterator(obj_, ind_ + diff);
            res.check_correct();
            return res;
        }

        const_iterator operator-(size_t diff) {
            const_iterator res = const_iterator(obj_, ind_ - diff);
            res.check_correct();
            return res;
        }

        const_iterator& operator++() {
            ind_++;
            check_correct();
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator res = const_iterator(obj_, ind_++);
            check_correct();
            return res;
        }

        const_iterator& operator--() {
            ind_--;
            check_correct();
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator res = const_iterator(obj_, ind_--);
            check_correct();
            return res;
        }

        const T& operator*() const {
            check_correct();
            return obj_->data_[ind_];
        }
        const T* operator->() const {
            check_correct();
            return obj_->data_ + ind_;
        }

        bool operator==(const const_iterator& anoth) const {
            return obj_ == anoth.obj_ && ind_ == anoth.ind_;
        }

        bool operator!=(const const_iterator& anoth) const {
            return obj_ != anoth.obj_ || ind_ != anoth.ind_;
        }

        bool operator<(const const_iterator& anoth) const {
            check_obj(anoth);
            return ind_ < anoth.ind_;
        }

        bool operator>(const const_iterator& anoth) const {
            check_obj(anoth);
            return ind_ > anoth.ind_;
        }

        bool operator<=(const const_iterator& anoth) const {
            check_obj(anoth);
            return ind_ <= anoth.ind_;
        }

        bool operator>=(const const_iterator& anoth) const {
            check_obj(anoth);
            return ind_ >= anoth.ind_;
        }

    private:
        void check_obj(const const_iterator& anoth) const {
            if (obj_ != anoth.obj_) {
                throw std::exception();
            }
        }

        void check_correct() const {
            if (obj_ == nullptr) {
                throw std::exception();
            }
            if (ind_ > obj_->size_) {
                throw std::exception();
            }
        }

        const my_vector* obj_ = nullptr;
        size_t ind_ = 0;
    };

    my_vector() {
    }

    my_vector(const my_vector& anoth) {
        realloc(anoth.size_, anoth.size_, anoth, anoth.size_);
    }

    my_vector(my_vector&& anoth) {
        swap(anoth);
    }

    my_vector(size_t size, const T& val = T()) {
        realloc(size, size, val);
    }

    my_vector(std::initializer_list<T> list) {
        realloc(list.size(), list.size(), list, list.size());
    }

    ~my_vector() = default;

    my_vector& operator=(const my_vector& anoth) {
        realloc(anoth.size_, anoth.size_, anoth, anoth.size_);
        return *this;
    }

    my_vector& operator=(my_vector&& anoth) {
        clear();
        swap(anoth);
        return *this;
    }

    void resize(size_t new_size) {
        realloc(new_size, new_size, *this, std::min(size_, new_size));
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            realloc(size_, new_capacity, *this, size_);
        }
    }

    void shrink_to_fit() {
        realloc(size_, size_, *this, size_);
    }

    void assign(size_t new_size, const T& val) {
        realloc(new_size, new_size, val);
    }

    void push_back(const T& val) {
        if (size_ == capacity_) {
            realloc(size_, new_capacity(), *this, size_);
        }
        data_[size_++] = val;
    }

    template <class... Args>
    void emplace_back(Args... args) {
        if (size_ == capacity_) {
            realloc(size_, new_capacity(), *this, size_);
        }
        data_[size_++] = T(std::forward<Args>(args)...);
    }

    void pop_back() {
        if (size_ == 0) {
            throw std::exception();
        }
        size_--;
    }

    T& operator[](size_t ind) {
        if (ind >= size_) {
            throw std::exception();
        }
        return data_[ind];
    }

    T operator[](size_t ind) const {
        if (ind >= size_) {
            throw std::exception();
        }
        return data_[ind];
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    Allocator get_allocator() const {
        return Allocator();
    }

    void swap(my_vector& anoth) {
        data_.swap(anoth.data_);
        std::swap(size_, anoth.size_);
        std::swap(capacity_, anoth.capacity_);
    }

    void clear() {
        data_.reset();
        size_ = 0;
        capacity_ = 0;
    }

    T& back() {
        if (size_ == 0) {
            throw std::exception();
        }
        return data_[size_ - 1];
    }

    T back() const {
        if (size_ == 0) {
            throw std::exception();
        }
        return data_[size_ - 1];
    }

    T& front() {
        if (size_ == 0) {
            throw std::exception();
        }
        return data_[0];
    }

    T front() const {
        if (size_ == 0) {
            throw std::exception();
        }
        return data_[0];
    }

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, size_);
    }

    const_iterator begin() const {
        return const_iterator(this, 0);
    }

    const_iterator end() const {
        return const_iterator(this, size_);
    }

    T* data() {
        return data_.get();
    }

    const T* data() const {
        return data_.get();
    }

    void insert(iterator it, const T& val) {
        if (it.ind_ > size_) {
            throw std::exception();
        }
        emplace_back();
        for (size_t i = size_ - 1; i > it.ind_; --i) {
            data_[i] = data_[i - 1];
        }
        data_[it.ind_] = val;
    }

    template <class... Args>
    void emplace(iterator it, Args... args) {
        if (it.ind_ > size_) {
            throw std::exception();
        }
        emplace_back();
        for (size_t i = size_ - 1; i > it.ind_; --i) {
            data_[i] = data_[i - 1];
        }
        data_[it.ind_] = T(std::forward<Args>(args)...);
    }

    void erase(iterator it) {
        if (it.ind_ >= size_) {
            throw std::exception();
        }
        for (size_t i = it.ind_; i + 1 < size_; ++i) {
            data_[i] = data_[i + 1];
        }
        pop_back();
    }

    bool operator==(const my_vector& anoth) {
        if (size_ != anoth.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != anoth.data_[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const my_vector& anoth) {
        if (size_ != anoth.size_) {
            return true;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != anoth.data_[i]) {
                return true;
            }
        }

        return false;
    }

private:
    void realloc(size_t new_size, size_t new_capacity, const T& val = T()) {
        std::unique_ptr<T[]> new_data = nullptr;
        if (new_capacity == 0) {
            data_.swap(new_data);
            return;
        }
        new_data.reset(Allocator().allocate(new_capacity));
        for (int i = 0; i < new_size; ++i) {
            new_data[i] = val;
        }

        data_.swap(new_data);
        size_ = new_size;
        capacity_ = new_capacity;
    }

    template <class C>
    void realloc(size_t new_size, size_t new_capacity, const C& copy_from, size_t copy_size) {
        std::unique_ptr<T[]> new_data = nullptr;
        if (new_capacity == 0) {
            data_.swap(new_data);
            return;
        }
        new_data.reset(Allocator().allocate(new_capacity));
        auto it = copy_from.begin();
        for (size_t i = 0; i < copy_size; ++i) {
            new_data[i] = *it;
            ++it;
        }
        for (size_t i = copy_size; i < new_size; ++i) {
            new_data[i] = T();
        }

        data_.swap(new_data);
        size_ = new_size;
        capacity_ = new_capacity;
    }

    static const int factor_;

    size_t new_capacity() {
        if (capacity_ == 0) {
            return 1;
        }
        return capacity_ * factor_;
    }

    std::unique_ptr<T[]> data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

template <class T, class A>
const int my_vector<T, A>::factor_ = 2;