#pragma once
#include <type_traits>
#include <giygas/export.h>

namespace giygas {

    // TODO: Should this be an enum class? Writting the extra operators to suppport bit opeartions
    // is kinda cumbersome.
    enum class AttachmentType {
        Color       = 1,
        Depth       = 1 << 1,
        Stencil     = 1 << 2
    };

    using T = std::underlying_type<AttachmentType>::type;

    GIYGAS_EXPORT AttachmentType operator|(AttachmentType lhs, AttachmentType rhs);
    GIYGAS_EXPORT AttachmentType operator&(AttachmentType lhs, AttachmentType rhs);
    GIYGAS_EXPORT AttachmentType &operator|=(AttachmentType& lhs, AttachmentType rhs);
    GIYGAS_EXPORT bool has_flag(AttachmentType lhs, AttachmentType rhs);

}
