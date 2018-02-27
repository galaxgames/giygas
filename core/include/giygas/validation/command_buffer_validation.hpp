#pragma once
#include <giygas/CommandPool.hpp>
#include <giygas/export.h>
#include <giygas/RendererType.hpp>
#include <giygas/CommandBuffer.hpp>

namespace giygas {
namespace validation {

    GIYGAS_EXPORT bool validate_command_buffer_create(
        const CommandBuffer *target,
        const CommandPool *pool
    );

    GIYGAS_EXPORT bool validate_command_buffer_record_pass(
        const CommandBuffer *target,
        const SingleBufferPassInfo &info
    );

}}
