#include <giygas/AttachmentType.hpp>

namespace giygas {
    AttachmentType operator|(AttachmentType lhs, AttachmentType rhs) {
        return static_cast<AttachmentType>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    AttachmentType operator&(AttachmentType lhs, AttachmentType rhs) {
        return static_cast<AttachmentType>(static_cast<T>(lhs) & static_cast<T>(rhs));
    }

    AttachmentType &operator|=(AttachmentType& lhs, AttachmentType rhs) {
        lhs = static_cast<AttachmentType>(static_cast<T>(lhs) | static_cast<T>(rhs));
        return lhs;
    }

    bool has_flag(AttachmentType lhs, AttachmentType rhs) {
        return static_cast<T>(lhs & rhs) > 0;
    }
}
