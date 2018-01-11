#pragma once
#include <stack>
#include <mutex>

namespace giygas {
    using namespace std;

    class TypelessPool {
    public:
        virtual ~TypelessPool() = default;
        virtual void give(void *T) = 0;
    };

    template <typename T>
    class Pool : public TypelessPool {
        stack<T *> _items;
        mutex _mutex;
    public:
        ~Pool();
        void give(T *item);
        void give(void *item) override;
        T *take();
    };

}