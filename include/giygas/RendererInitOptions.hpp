#pragma once
#include <giygas/export.h>

namespace giygas {

    enum class GIYGAS_EXPORT PolygonCullingMode {
        BackFace,
        FrontFace
    };

    enum class GIYGAS_EXPORT VertexWinding {
        CounterClockwise,
        Clockwise
    };

    class GIYGAS_EXPORT RendererInitOptions {
    public:
        bool polygon_culling_enabled;
        PolygonCullingMode polygon_culling_mode;
        VertexWinding  front_face_vertex_winding;

        RendererInitOptions(bool polygon_culling_enabled);
        RendererInitOptions(
            PolygonCullingMode polygon_culling_mode,
            VertexWinding front_face_vertex_winding
        );
    };
}