#include "giygas_internal/GLMaterial.hpp"
#include "giygas_internal/GLShader.hpp"
#include <memory>
#include <cstring>
#include <cassert>

using namespace giygas;
using namespace std;

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
    _gl = other._gl;
    _program = other._program;
    _is_valid = other._is_valid;
    _message = other._message;
    _texture_count = other._texture_count;

    other._program = 0;
    other._message = nullptr;
}

GLMaterial& GLMaterial::operator=(GLMaterial &&other) noexcept {
    _shader = move(other._shader);
    _values = move(other._values);
    _locations = move(other._locations);
    _textures = move(other._textures);

    _gl = other._gl;
    _program = other._program;
    _is_valid = other._is_valid;
    _message = other._message;
    _texture_count = other._texture_count;

    other._program = 0;
    other._message = nullptr;

    return *this;
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
    template <>
    UniformValue* GLMaterial::make_value<float>(float value) {
        return new FloatUniformValue(value);
    }

    template <>
    UniformValue* GLMaterial::make_value<int>(int value) {
        return new IntUniformValue(value);
    }
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
        value_controller->do_gl_call(_gl, location);
    }
}

template void GLMaterial::set_uniform<float>(const string &name, float value);
template void GLMaterial::set_uniform<int>(const string &name, int value);

void GLMaterial::set_uniform_float(const std::string &name, float value) {
    set_uniform(name, value);
}

void GLMaterial::set_uniform_texture(
    const std::string &name, size_t index
) {
    set_uniform(name, static_cast<int>(index));
}

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

    //if (auto shader = _shader.lock()) {
//        assert(shader->renderer_type() == RendererType::OpenGL);
//        GLShader *gl_shader = reinterpret_cast<GLShader*>(shader.get());
        return _gl->get_uniform_location(_program, name.c_str());
//    }
//    else {
//        return -1;
//    }
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

FloatUniformValue::FloatUniformValue(float value) {
    _value = value;
}

void FloatUniformValue::do_gl_call(GL *gl, GLint location) {
    gl->uniform_1f(location, _value);
}

IntUniformValue::IntUniformValue(int value) {
    _value = value;
}

void IntUniformValue::do_gl_call(GL *gl, GLint location) {
    gl->uniform_1i(location, _value);
}

