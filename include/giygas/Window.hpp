#pragma once
#include <giygas/export.h>
#include <functional>

namespace giygas {

    class SurfaceSizeChangedListener {
    public:
        virtual ~SurfaceSizeChangedListener() = default;
        virtual void handle_surface_size_changed(
            unsigned int width,
            unsigned int height
        ) = 0;
    };

    class GIYGAS_EXPORT Window {
    public:
        virtual ~Window() = default;
        virtual bool is_valid() const = 0;
        virtual unsigned int framebuffer_width() const = 0;
        virtual unsigned int framebuffer_height() const = 0;
        virtual void add_surface_size_changed_listener(
            SurfaceSizeChangedListener *listener
        ) = 0;
        virtual void remove_surface_size_changed_listener(
            SurfaceSizeChangedListener *listener
        ) = 0;
    };
}
