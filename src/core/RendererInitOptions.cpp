#include <giygas/RendererInitOptions.hpp>

using namespace giygas;

RendererInitOptions::RendererInitOptions(bool polygon_culling_enabled)
    : polygon_culling_enabled(polygon_culling_enabled)
{
    polygon_culling_mode = PolygonCullingMode::BackFace;
    front_face_vertex_winding = VertexWinding::CounterClockwise;
}

RendererInitOptions::RendererInitOptions(
    PolygonCullingMode polygon_culling_mode,
    VertexWinding front_face_vertex_winding
)
    : polygon_culling_mode(polygon_culling_mode)
    , front_face_vertex_winding(front_face_vertex_winding)
{
    polygon_culling_enabled = true;
}
