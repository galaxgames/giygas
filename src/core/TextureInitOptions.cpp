#include <giygas/TextureInitOptions.hpp>

using namespace giygas;

TextureInitOptions::TextureInitOptions() {
    wrap_mode_u = TextureWrapMode::ClampToEdge;
    wrap_mode_v = TextureWrapMode::ClampToEdge;
    minify_filter_mode = TextureMinifyFilterMode::Linear;
    magnify_filter_mode = TextureMagnifyFilterMode::Linear;
}

TextureInitOptions::TextureInitOptions(
    TextureWrapMode wrap_mode,
    TextureMinifyFilterMode minify_filter_mode,
    TextureMagnifyFilterMode magnify_filter_mode
)
    : minify_filter_mode(minify_filter_mode)
    , magnify_filter_mode(magnify_filter_mode)
{
    wrap_mode_u = wrap_mode;
    wrap_mode_v = wrap_mode;
}

TextureInitOptions::TextureInitOptions(
    TextureWrapMode wrap_mode_u,
    TextureWrapMode wrap_mode_v,
    TextureMinifyFilterMode minify_filter_mode,
    TextureMagnifyFilterMode magnify_filter_mode
)
    : wrap_mode_u(wrap_mode_u)
    , wrap_mode_v(wrap_mode_v)
    , minify_filter_mode(minify_filter_mode)
    , magnify_filter_mode(magnify_filter_mode)
{}
