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

    enum class GIYGAS_EXPORT DepthFunction {
        PassNever,
        PassLess,
        PassEqual,
        PassLessThanOrEqual,
        PassGreater,
        PassGreaterThanOrEqual,
        PassNotEqual,
        PassAlways
    };

    class GIYGAS_EXPORT PolygonCullingOptions {
    public:
        bool enabled;
        PolygonCullingMode mode;
        VertexWinding  front_face_vertex_winding;

        PolygonCullingOptions(bool enabled);
        PolygonCullingOptions(
            PolygonCullingMode mode,
            VertexWinding front_face_vertex_winding
        );
    };

    class GIYGAS_EXPORT DepthBufferOptions {
    public:
        bool depth_test_enabled;
        bool mask_enabled;
        DepthFunction function;
        double range_near;
        double range_far;

        DepthBufferOptions(bool depth_test_enabled);
        DepthBufferOptions(DepthFunction function);

        DepthBufferOptions(
            DepthFunction function,
            double range_near,
            double range_far
        );

        DepthBufferOptions(
            bool depth_test_enabled,
            bool mask_enabled,
            DepthFunction function,
            double range_near,
            double range_far
        );

    };

    class GIYGAS_EXPORT RendererInitOptions {
    public:
        PolygonCullingOptions polygon_culling;
        DepthBufferOptions depth_buffer;

        RendererInitOptions();
        RendererInitOptions(PolygonCullingOptions polygon_culling);
        RendererInitOptions(DepthBufferOptions depth_buffer);

        RendererInitOptions(
            PolygonCullingOptions polygon_culling,
            DepthBufferOptions depth_buffer
        );
    };
}