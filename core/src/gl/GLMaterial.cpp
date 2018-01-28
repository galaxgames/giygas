#include "GLMaterial.hpp"
#include "operations/CreateProgramGLOperation.hpp"
#include "operations/DeleteProgramGLOperation.hpp"
#include "GLRenderer.hpp"
#include "operations/LinkProgramGLOperation.hpp"
#include "GLUniformValue.hpp"
#include "operations/GetUniformLocationGLOperation.hpp"
#include "operations/SetUniformValueGLOperation.hpp"
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
    GLUniformValue *GLMaterial::make_value<T>(T value) { \
        return new uniform_value_t(value);\
    }


GLMaterial::GLMaterial(GLRenderer *renderer) :
    _textures(new weak_ptr<Texture>[0])
{
    _renderer = renderer;
    _is_valid = false;
    _texture_count = 0;

    CreateProgramGLOperation create_op;
    _renderer->add_operation_and_notify(&create_op, nullptr);
    _program = create_op.get_handle();
}

GLMaterial::GLMaterial(GLMaterial &&other) noexcept :
    //_shader(move(other._shader)),
    _values(move(other._values)),
    _locations(move(other._locations)),
    _textures(move(other._textures))
{
    move_common(move(other));
}

GLMaterial& GLMaterial::operator=(GLMaterial &&other) noexcept {
    //_shader = move(other._shader);
    _values = move(other._values);
    _locations = move(other._locations);
    _textures = move(other._textures);
    move_common(move(other));
    return *this;
}

void GLMaterial::move_common(GLMaterial &&other) noexcept {
    _renderer = other._renderer;
    _program = other._program;
    _is_valid = other._is_valid;
    _message = other._message;
    _texture_count = other._texture_count;
    other._program = 0;
    other._message = nullptr;
}

GLMaterial::~GLMaterial() {
    Pool<DeleteProgramGLOperation> &pool
        = _renderer->pools().delete_program_ops;
    DeleteProgramGLOperation *delete_op = pool.take();
    delete_op->set(_program);
    _renderer->add_operation(delete_op, &pool);
}

RendererType GLMaterial::renderer_type() const {
    return RendererType::OpenGL;
}

void GLMaterial::link_shaders(const Shader **shaders, size_t count) {
    //void GLMaterial::set_shader(weak_ptr<Shader> shader) {
    //_shader = shader;

    _locations.clear();

    _is_valid = false;

    //delete[] _message;


    GLuint vertex_handle = 0;
    GLuint fragment_handle = 0;

    for (size_t i = 0; i < count; ++i) {
        const Shader *shader = shaders[i];
        assert(shader->renderer_type() == RendererType::OpenGL);
        const auto *gl_shader = reinterpret_cast<const GLShader *>(shader);

        switch (gl_shader->shader_type()) {
            case ShaderType::Vertex:
                vertex_handle = gl_shader->handle();
                break;
            case ShaderType::Fragment:
                fragment_handle = gl_shader->handle();
                break;
        }
    }

    LinkProgramGLOperation link_op(
        _program,
        vertex_handle,
        fragment_handle
    );
    _renderer->add_operation_and_notify(&link_op, nullptr);
    link_op.wait();

    _is_valid = link_op.is_success();
    _message = move(link_op.message());

    if (_is_valid) {
        Pool<SetUniformValueGLOperation> &pool
            = _renderer->pools().set_uniform_value_ops;
        for (auto& pair : _values) {
            GLint location = get_location(pair.first);
            _locations.insert(make_pair(pair.first, location));
            SetUniformValueGLOperation *set_op = pool.take();
            set_op->set(
                _program,
                location,
                pair.second.get()
            );
            _renderer->add_operation(set_op, &pool);
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

    // TODO: Re-use previous value controller if uniform was previously set
    // to the same type (which it usually is)
    GLUniformValue *value_controller = make_value<T>(value);

    auto it = _values.find(name);
    if (it == _values.end()) {
        _values.insert(make_pair(
            name, unique_ptr<GLUniformValue>(value_controller)
        ));
    }
    else {
        it->second = unique_ptr<GLUniformValue>(value_controller);
    }

    if (_is_valid && location != -1) {
        Pool<SetUniformValueGLOperation> &pool
            = _renderer->pools().set_uniform_value_ops;
        SetUniformValueGLOperation *set_op = pool.take();
        // TODO: WOAH!! something bad is going to happen here.
        // Need a different GLOperation per type of uniform.
        // Otherwise, we might be accesing the passed in value controller after
        // deleting it (!!!)
        set_op->set(_program, location, value_controller);
        _renderer->add_operation(set_op, &pool);
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

const string &GLMaterial::get_message() const {
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
    GetUniformLocationGLOperation op(_program, name.c_str());
    _renderer->add_operation_and_notify(&op, nullptr);
    return op.get_location();
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
