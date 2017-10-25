#include "giygas_internal/GLMaterial.hpp"
#include "giygas_internal/GLShader.hpp"
#include <memory>
#include <cstring>
#include <cassert>

using namespace giygas;
using namespace std;

#define UNIFORM_FUNC(func_name, T) \
    void GLMaterial::func_name(const string &name, T value)


#define UNIFORM_FUNC_IMPL(func_name, T) \
    UNIFORM_FUNC(func_name, T) {\
        set_uniform<T>(name, value);\
    } \
    template void GLMaterial::set_uniform<T>(const string &name, T value);


#define MAKE_VALUE_IMPL(T, uniform_value_t) \
    template <> \
    UniformValue *GLMaterial::make_value<T>(T value) { \
        return new uniform_value_t(value);\
    }


#define UNIFORM_VALUE_IMPL(class_name, T, impl) \
    class_name::class_name(T value) \
        : _value(value) \
    { \
    } \
    \
    void class_name::do_gl_call(GL *gl, GLint location) impl


GLMaterial::GLMaterial(GL *gl) :
    _textures(new weak_ptr<Texture>[0])
{
    _gl = gl;
    _program = gl->create_program();
    _is_valid = false;
    _message = nullptr;
    _texture_count = 0;
}

GLMaterial::GLMaterial(GLMaterial &&other) noexcept :
    _shader(move(other._shader)),
    _values(move(other._values)),
    _locations(move(other._locations)),
    _textures(move(other._textures))
{
    move_common(move(other));
}

GLMaterial& GLMaterial::operator=(GLMaterial &&other) noexcept {
    _shader = move(other._shader);
    _values = move(other._values);
    _locations = move(other._locations);
    _textures = move(other._textures);
    move_common(move(other));
    return *this;
}

void GLMaterial::move_common(GLMaterial &&other) noexcept {
    _gl = other._gl;
    _program = other._program;
    _is_valid = other._is_valid;
    _message = other._message;
    _texture_count = other._texture_count;
    other._program = 0;
    other._message = nullptr;
}

GLMaterial::~GLMaterial() {
    delete[] _message;
    _gl->delete_program(_program);
}

RendererType GLMaterial::renderer_type() const {
    return RendererType::OpenGL;
}

void GLMaterial::set_shader(weak_ptr<Shader> shader) {
    _shader = shader;
    _locations.clear();

    _is_valid = false;

    delete[] _message;

    if (auto shared_shader = shader.lock()) {
        assert(shared_shader->renderer_type() == RendererType::OpenGL);
        auto *gl_shader = reinterpret_cast<GLShader *>(shared_shader.get());
        _gl->attach_shader(_program, gl_shader->get_vertex_shader());
        _gl->attach_shader(_program, gl_shader->get_fragment_shader());
        _gl->link_program(_program);

        GLint status;
        _gl->get_program_iv(_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint log_length;
            GLint log_offset;
            GLsizei retrieved;
            _gl->get_program_iv(_program, GL_INFO_LOG_LENGTH, &log_length);
            _message = new char[log_length];
            while (log_offset < log_length - 1) {
                _gl->get_program_info_log(
                    _program,
                    log_length - log_offset,
                    &retrieved,
                    _message + log_offset
                );
                log_offset += retrieved;
            }
            _message[log_length - 1] = '\0';
        }
        else {
            const char *no_error_message = "No error";
            // TODO: get length of no error message string at compile time
            _message = new char[strlen(no_error_message) + 1];
            strcpy(_message, no_error_message);
            _is_valid = true;
        }
    }

    if (_is_valid) {
        _gl->use_program(_program);
        for (auto& pair : _values) {
            GLint location = get_location(pair.first);
            _locations.insert(make_pair(pair.first, location));
            pair.second->do_gl_call(_gl, location);
        }
    }

}

void GLMaterial::set_textures(const weak_ptr<Texture> *textures, size_t count) {
    _textures = unique_ptr<weak_ptr<Texture>[]>(new weak_ptr<Texture>[count]);
    _texture_count = count;
    for (size_t i = 0; i < count; ++i) {
        _textures[i] = textures[i];
    }
}

namespace giygas {
    MAKE_VALUE_IMPL(float, FloatUniformValue)
    MAKE_VALUE_IMPL(int, IntUniformValue)
    MAKE_VALUE_IMPL(Vector2, Vector2UniformValue)
    MAKE_VALUE_IMPL(Vector3, Vector3UniformValue)
    MAKE_VALUE_IMPL(Vector4, Vector4UniformValue)
    MAKE_VALUE_IMPL(const Matrix4x4 &, Matrix4x4UniformValue)
}

template <typename T>
void GLMaterial::set_uniform(const string &name, T value) {
    GLint location = get_cached_location(name);
    if (location == -1) {
        location = get_location(name);
        if (location != -1) {
            _locations.insert(make_pair(name, location));
        }
    }
    
    UniformValue *value_controller = make_value<T>(value);

    auto it = _values.find(name);
    if (it == _values.end()) {
        _values.insert(make_pair(
            name, unique_ptr<UniformValue>(value_controller)
        ));
    }
    else {
        it->second = unique_ptr<UniformValue>(value_controller);
    }

    if (_is_valid && location != -1) {
        _gl->use_program(_program);
        value_controller->do_gl_call(_gl, location);
    }
}

UNIFORM_FUNC_IMPL(set_uniform_float, float)
UNIFORM_FUNC_IMPL(set_uniform_vector2, Vector2)
UNIFORM_FUNC_IMPL(set_uniform_vector3, Vector3)
UNIFORM_FUNC_IMPL(set_uniform_vector4, Vector4)
UNIFORM_FUNC_IMPL(set_uniform_matrix4x4, const Matrix4x4 &)

UNIFORM_FUNC(set_uniform_texture, size_t) {
    set_uniform(name, static_cast<int>(value));
}
template void GLMaterial::set_uniform<int>(const string &name, int value);


bool GLMaterial::is_valid() const {
    return _is_valid;
}

const char* GLMaterial::get_message() const {
    return _message;
}

GLint GLMaterial::get_cached_location(const string &name) const {
    auto it = _locations.find(name);
    if (it != _locations.end()) {
        return it->second;
    }
    return -1;
}

GLint GLMaterial::get_location(const string &name) const {
    if (!_is_valid) {
        return -1;
    }
    return _gl->get_uniform_location(_program, name.c_str());
}

GLuint GLMaterial::get_program() const {
    return _program;
}

size_t GLMaterial::get_texture_count() const {
    return _texture_count;
}

weak_ptr<Texture> GLMaterial::get_texture(size_t i) const {
    assert(i < _texture_count);
    return _textures[i];
}

// *****
// TODO: Move these into new files
// *****

UNIFORM_VALUE_IMPL(FloatUniformValue, float, {
    gl->uniform_1f(location, _value);
})

UNIFORM_VALUE_IMPL(IntUniformValue, int, {
    gl->uniform_1i(location, _value);
})

UNIFORM_VALUE_IMPL(Vector2UniformValue, Vector2, {
    gl->uniform_2f(location, _value.x, _value.y);
})

UNIFORM_VALUE_IMPL(Vector3UniformValue, Vector3, {
    gl->uniform_3f(location, _value.x, _value.y, _value.z);
})

UNIFORM_VALUE_IMPL(Vector4UniformValue, Vector4, {
    gl->uniform_4f(location, _value.x, _value.y, _value.z, _value.w);
})

Matrix4x4UniformValue::Matrix4x4UniformValue(const Matrix4x4 &value)
    :_value(value)
{
}

void Matrix4x4UniformValue::do_gl_call(GL *gl, GLint location) {
    gl->uniform_matrix_4fv(
        location, 1, false, reinterpret_cast<float *>(&_value)
    );
}
