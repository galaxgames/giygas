#pragma once

namespace giygas {

    // TODO: Make this an enum class
    enum ColorChannels {
        GIYGAS_COLOR_CHANNELS_RED = 1 << 0,
        GIYGAS_COLOR_CHANNELS_GREEN = 1 << 1,
        GIYGAS_COLOR_CHANNELS_BLUE = 1 << 2,
        GIYGAS_COLOR_CHANNELS_ALPHA = 1 << 3
    };

}