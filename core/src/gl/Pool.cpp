#include "Pool.hpp"

using namespace giygas;

template <typename T>
Pool<T>::~Pool() {
    while (!_items.empty()) {
        delete _items.top();
        _items.pop();
    }
}

template <typename T>
void Pool<T>::give(T *item) {
    lock_guard<mutex> lock(_mutex);
    _items.push(item);
}

template <typename T>
void Pool<T>::give(void *item) {
    lock_guard<mutex> lock(_mutex);
    _items.push(reinterpret_cast<T *>(item));
}

template <typename T>
T *Pool<T>::take() {
    lock_guard<mutex> lock(_mutex);
    if (_items.empty()) {
        return new T();
    }
    T *item = _items.top();
    _items.pop();
    return item;
}
