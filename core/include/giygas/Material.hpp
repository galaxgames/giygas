#pragma once
#include <memory>
#include <giygas/export.h>
#include <giygas/Shader.hpp>
#include "Texture.hpp"
#include "RendererType.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4x4.hpp"
#include "Sampler.hpp"

namespace giygas {
    using namespace std;

    class GIYGAS_EXPORT Material {
    public:
        virtual ~Material() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_textures(
            const Texture **textures,
            const Sampler** samplers,
            size_t count
        ) = 0;

        virtual void reset(size_t size) = 0;
        virtual void set_uniform_float(size_t offset, float value) = 0;
        virtual void set_uniform_texture(size_t offset, uint32_t value) = 0;
        virtual void set_uniform_vector2(size_t offset, Vector2 value) = 0;
        virtual void set_uniform_vector3(size_t offset, Vector3 value) = 0;
        virtual void set_uniform_vector4(size_t offset, Vector4 value) = 0;
        virtual void set_uniform_matrix4x4(size_t offset, const Matrix4x4 &value) = 0;
    };
}
