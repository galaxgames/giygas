#ifndef GIYGAS_EVENTHANDLER_HPP
#define GIYGAS_EVENTHANDLER_HPP
#include <unordered_set>
#include <vector>
#include <functional>

namespace giygas {

    enum class EventHandlerOpType {
        Add,
        Remove
    };

    template <typename ... P> class EventHandler;


    template <typename ... P>
    class Event {
        typedef EventHandler<P...> handler_t;

        std::unordered_set<handler_t *> _handlers;
        std::vector<std::pair<EventHandlerOpType, handler_t *>> _ops;

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

        void process_ops() {
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
        }

    public:
        Event() = default;
        Event(const Event &other) = delete;
        const Event &operator=(const Event &other) = delete;

        Event(Event &&other) noexcept
            : _handlers(move(other._handlers))
            , _ops(move(other._ops))
        {
            move_common(std::move(other));
        }

        ~Event() {
            for (const auto &handler_it : _handlers) {
                handler_it->_event = nullptr;
            }
        }

        Event &operator=(Event &&other) noexcept {
            _handlers = move(other._handlers);
            _ops = move(other._ops);
            move_common(std::move(other));
            return *this;
        }

        void add(handler_t *handler) {
            _ops.emplace_back(EventHandlerOpType::Add, handler);
        }

        void remove(handler_t *handler) {
            _ops.emplace_back(EventHandlerOpType::Remove, handler);
        }

        bool has_handlers() {
            process_ops();
            return !_handlers.empty();
        }

        handler_t make_handler();

        template <typename ... ForwardP>
        void invoke(ForwardP&& ... args) {
            process_ops();

            for (auto *handler : _handlers) {
                handler->delegate(std::forward<P>(args)...);
            }
        }
    };

    template <typename ... P>
    class EventHandler {
        friend class Event<P...>;

        void move_common(EventHandler<P...> &&other) noexcept {

            if (_event != nullptr) {
                _event->remove(this);
                _event->add(&other);
            }

            if (other._event != nullptr) {
                other._event->remove(&other);
                other._event->add(this);
            }

            std::swap(_event, other._event);
        }

    protected:
        // TODO: Is there a better way to do this that doesn't require friendship?
        Event<P...> *_event = nullptr;
    public:
        std::function<void(P...)> delegate;

        EventHandler() = default;

        explicit EventHandler(Event<P...> *event) {
            _event = event;
        }

        EventHandler(const EventHandler &other) = delete;
        const EventHandler &operator=(const EventHandler &other) = delete;

        EventHandler(EventHandler &&other) noexcept
            : delegate(move(other.delegate))
        {
            move_common(std::move(other));
        }

        EventHandler &operator=(EventHandler &&other) noexcept {
            delegate = move(other.delegate);
            move_common(std::move(other));
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

#endif // GIYGAS_EVENTHANDLER_HPP

#ifdef GIYGAS_EVENT_INCLUDE_HELPERS

#define BIND_MEMBER0(fn) std::bind(fn, this)

#define BIND_MEMBER1(fn) std::bind(fn, this, \
    std::placeholders::_1);

#define BIND_MEMBER2(fn) std::bind(fn, this, \
    std::placeholders::_1, std::placeholders::_2);

#define BIND_MEMBER3(fn) std::bind(fn, this, \
    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

#define BIND_MEMBER4(fn) std::bind(fn, this, \
    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

#define BIND_MEMBER5(fn) std::bind(fn, this, \
    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 \
    std::placeholders::_5);

#define BIND_MEMBER6(fn) std::bind(fn, this, \
    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 \
    std::placeholders::_5, std::placeholders::_6);

#define BIND_MEMBER7(fn) std::bind(fn, this, \
    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 \
    std::placeholders::_5, std::placeholders::_6, std::placeholders::_7);

#define BIND_MEMBER8(fn) std::bind(fn, this \
    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4 \
    std::placeholders::_5, std::placeholders::_6, std::placeholders::_7, std::placeholders::_8);

#endif
