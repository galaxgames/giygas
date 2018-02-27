#pragma once

#include <giygas/Pipeline.hpp>

namespace giygas {
namespace validation {

    GIYGAS_EXPORT bool validate_pipeline_create(
        const Pipeline *target,
        const PipelineCreateParameters &params
    );

}}
