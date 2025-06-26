#ifndef ATOMICVECTOR_H
#define ATOMICVECTOR_H

#include <atomic>

template <typename T>
class custom_atomic {
    public:

    // Custom copy constructor
    custom_atomic(const custom_atomic<T>& other) {
        value.store(other.value.load());
    }

    // Default constructor
    custom_atomic() = default;
    custom_atomic(T new_value) {
        value.store(new_value);
    }

    T load(){
        return value.load();
    }

    void store(T new_value){
        value.store(new_value);
    }

    private:
    std::atomic<T> value; // use 'initializing' constructor

};

typedef custom_atomic<float> atomic_float;
typedef custom_atomic<int> atomic_int;
typedef custom_atomic<bool> atomic_bool;

#endif //ATOMICVECTOR_H