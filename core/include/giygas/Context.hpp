#pragma once
#include <functional>
#include <giygas/export.h>
#include "RendererType.hpp"
#include "EventHandler.hpp"
#include "InputKey.hpp"

namespace giygas {

    class GIYGAS_EXPORT Context {
    public:
        virtual ~Context() = default;

        virtual bool is_valid() const = 0;
        virtual void *cast_to_specific(RendererType type) = 0;
        virtual void update() = 0;
        virtual bool should_close() const = 0;
        virtual unsigned int translate_key(InputKey key) const = 0;

        virtual EventHandler<unsigned int, float> input_changed() = 0;
    };
}
