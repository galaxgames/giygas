#pragma once
#include <giygas/export.h>
#include <giygas/RendererType.hpp>
#include <giygas/submission.hpp>

namespace giygas {
namespace validation {

    GIYGAS_EXPORT bool validate_submission_passes(
            const PassSubmissionInfo *passes
            , uint32_t pass_count
            , RendererType renderer_type);

}}
