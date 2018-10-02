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
        virtual uint32_t translate_key(InputKey key) const = 0;
        virtual InputKey get_universal_key(uint32_t key) const = 0;
        virtual float get_input(uint32_t input) const = 0;

        virtual EventHandler<uint32_t, float> input_changed() = 0;
    };
}
