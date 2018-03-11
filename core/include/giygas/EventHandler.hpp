#pragma once
#include <unordered_set>
#include <vector>
#include <functional>

namespace giygas {
    using namespace std;

    enum class EventHandlerOpType {
        Add,
        Remove
    };

    template <typename ... P> class EventHandler;


    template <typename ... P>
    class Event {
        typedef EventHandler<P...> handler_t;

        unordered_set<handler_t *> _handlers;
        vector<pair<EventHandlerOpType, handler_t *>> _ops;

        void move_common(Event &&other) noexcept {
            for (handler_t *handler : _handlers) {
                if (handler->_event == &other) {
                    handler->_event = this;
                }
            }
            for (auto pair : _ops) {
                if (pair.second->_event == &other) {
                    pair.second->_event = this;
                }
            }
        }

    public:
        Event() = default;
        Event(const Event &other) = delete;
        const Event &operator=(const Event &other) = delete;

        Event(Event &&other)
            : _handlers(move(other._handlers))
            , _ops(move(other._ops))
        {
            move_common(move(other));
        }

        Event &operator=(Event &&other) {
            _handlers = move(other._handlers);
            _ops = move(other._ops);
            move_common(move(other));
            return *this;
        }

        void add(handler_t *handler) {
            _ops.emplace_back(EventHandlerOpType::Add, handler);
        }

        void remove(handler_t *handler) {
            _ops.emplace_back(EventHandlerOpType::Remove, handler);
        }

        handler_t make_handler();

        template <typename ... ForwardP>
        void invoke(ForwardP&& ... args) {
            for (const auto &pair : _ops) {
                if (pair.first == giygas::EventHandlerOpType::Add) {
                    _handlers.emplace(pair.second);
                } else {
                    auto it = _handlers.find(pair.second);
                    if (it != _handlers.end()) {
                        _handlers.erase(it);
                    }
                }
            }
            _ops.clear();

            for (auto *handler : _handlers) {
                handler->delegate(forward<P>(args)...);
            }
        }
    };

    template <typename ... P>
    class EventHandler {
        friend class Event<P...>;

        void move_common(EventHandler<P...> &&other) noexcept {
            if (_event != nullptr) {
                _event->remove(this);
            }
            _event = other._event;
            if (other._event != nullptr) {
                other._event->remove(&other);
                other._event = nullptr;
            }
            if (_event != nullptr) {
                _event->add(this);
            }
        }

    protected:
        // TODO: Is there a better way to do this that doesn't require friendship?
        Event<P...> *_event;
    public:
        function<void(P...)> delegate;

        EventHandler() {
            _event = nullptr;
        }

        EventHandler(Event<P...> *event) {
            _event = event;
        }

        EventHandler(const EventHandler &other) = delete;
        const EventHandler &operator=(const EventHandler &other) = delete;

        EventHandler(EventHandler &&other)
            : delegate(move(other.delegate))
        {
            move_common(move(other));
        }

        EventHandler &operator=(EventHandler &&other) {
            delegate = move(other.delegate);
            move_common(move(other));
            return *this;
        }

        ~EventHandler() {
            if (_event != nullptr) {
                _event->remove(this);
            }
        }

    };


    template <typename ... P>
    EventHandler<P...> Event<P...>::make_handler() {
        EventHandler<P...> handler(this);
        add(&handler);
        return handler;
    }
}

