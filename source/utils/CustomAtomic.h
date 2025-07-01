#ifndef ATOMICVECTOR_H
#define ATOMICVECTOR_H

#include <atomic>

template <typename T>
class custom_atomic {
    public:

    // Custom copy constructor
    custom_atomic(const custom_atomic<T>& other) {
        T other_value = other.value.load(std::memory_order_relaxed);
        value.store(other_value,std::memory_order_relaxed);
    }

    // Default constructor
    custom_atomic() = default;
    custom_atomic(T new_value) {
        value.store(new_value, std::memory_order_relaxed);
    }

    T load(){
        return value.load(std::memory_order_relaxed);
    }
    

    void store(T new_value){
        value.store(new_value, std::memory_order_relaxed);
    }

    private:
    std::atomic<T> value; // use 'initializing' constructor

};

class atomic_bool {
    public:

    // Custom copy constructor
    atomic_bool(const atomic_bool& other) {
        bool other_value = other.value.load(std::memory_order_acquire);
        value.store(other_value,std::memory_order_release);
    }

    // Default constructor
    atomic_bool() = default;
    atomic_bool(bool new_value) {
        value.store(new_value, std::memory_order_release);
    }

    bool load(){
        return value.load(std::memory_order_acquire);
    }
    

    void store(bool new_value){
        value.store(new_value, std::memory_order_release);
    }

    void set(){
        value.store(true, std::memory_order_release);
    }

    // returns true if the bool was set, clears it afterwards
    bool is_set_clear(){
        return value.exchange(false, std::memory_order_acq_rel);
    }

    // bool exchange(bool new_value){
    //     value.exchange(new_value, std::memory)
    // }

    private:
    std::atomic<bool> value; // use 'initializing' constructor

};


typedef custom_atomic<float> atomic_float;
typedef custom_atomic<int> atomic_int;
// typedef custom_atomic<bool> atomic_bool;

#endif //ATOMICVECTOR_H