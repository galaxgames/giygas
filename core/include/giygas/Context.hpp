#pragma once
#include <functional>
#include <giygas/export.h>
#include "RendererType.hpp"
#include "EventHandler.hpp"

namespace giygas {

    class GIYGAS_EXPORT Context {
    public:
        virtual ~Context() = default;

        virtual bool is_valid() const = 0;
        virtual void *cast_to_specific(RendererType type) = 0;
        virtual unsigned int framebuffer_width() const = 0;
        virtual unsigned int framebuffer_height() const = 0;

        virtual EventHandler<unsigned int, unsigned int> surface_size_changed() = 0;
    };
}
