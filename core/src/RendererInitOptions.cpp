#include <giygas/RendererInitOptions.hpp>

using namespace giygas;

PolygonCullingOptions::PolygonCullingOptions(bool enabled)
    : enabled(enabled)
{
    mode = PolygonCullingMode::BackFace;
    front_face_vertex_winding = VertexWinding::CounterClockwise;
}

PolygonCullingOptions::PolygonCullingOptions(
    PolygonCullingMode mode,
    VertexWinding front_face_vertex_winding
)
    : mode(mode)
    , front_face_vertex_winding(front_face_vertex_winding)
{
    enabled = true;
}

DepthBufferOptions::DepthBufferOptions(bool depth_test_enabled)
    : depth_test_enabled(depth_test_enabled)
{
    mask_enabled = true;
    function = DepthFunction::PassLess;
    range_near = 0;
    range_far = 1;
}

DepthBufferOptions::DepthBufferOptions(DepthFunction function)
    : function(function)
{
    depth_test_enabled = true;
    mask_enabled = true;
    range_near = 0;
    range_far = 1;
}

DepthBufferOptions::DepthBufferOptions(
    DepthFunction function,
    double range_near,
    double range_far
)
    : function(function)
    , range_near(range_near)
    , range_far(range_far)
{
    depth_test_enabled = true;
    mask_enabled = true;
}

DepthBufferOptions::DepthBufferOptions(
    bool depth_test_enabled,
    bool mask_enabled,
    DepthFunction function,
    double range_near,
    double range_far
)
    : depth_test_enabled(depth_test_enabled)
    , mask_enabled(mask_enabled)
    , function(function)
    , range_near(range_near)
    , range_far(range_far)
{}

RendererInitOptions::RendererInitOptions()
    : polygon_culling(false)
    , depth_buffer(false)
{}

RendererInitOptions::RendererInitOptions(PolygonCullingOptions polygon_culling)
    : polygon_culling(polygon_culling)
    , depth_buffer(false)
{}

RendererInitOptions::RendererInitOptions(DepthBufferOptions depth_buffer)
    : polygon_culling(false)
    , depth_buffer(depth_buffer)
{}

RendererInitOptions::RendererInitOptions(
    PolygonCullingOptions polygon_culling,
    DepthBufferOptions depth_buffer
)
    : polygon_culling(polygon_culling)
    , depth_buffer(depth_buffer)
{}
