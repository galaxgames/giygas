#include "GLMaterial.hpp"
#include "GLShader.hpp"
#include <memory>

using namespace giygas;
using namespace std;

GLMaterial::GLMaterial() {
    _program = glCreateProgram();
    _is_valid = false;
    _message = nullptr;
    _next_texture_index = 0;
}

GLMaterial::GLMaterial(GLMaterial &&other) noexcept :
    _shader(move(other._shader)),
    _values(move(other._values)),
    _locations(move(other._locations)),
    _textures(move(other._textures))
{
    _program = other._program;
    _is_valid = other._is_valid;
    _message = other._message;
    _next_texture_index = other._next_texture_index;

    other._program = 0;
    other._message = nullptr;
}

GLMaterial& GLMaterial::operator=(GLMaterial &&other) noexcept {
    _shader = move(other._shader);
    _values = move(other._values);
    _locations = move(other._locations);
    _textures = move(other._textures);

    _program = other._program;
    _is_valid = other._is_valid;
    _message = other._message;
    _next_texture_index = other._next_texture_index;

    other._program = 0;
    other._message = nullptr;

    return *this;
}

GLMaterial::~GLMaterial() {
    delete[] _message;
    glDeleteProgram(_program);
}

void GLMaterial::set_shader(weak_ptr<Shader> shader) {
    _shader = shader;
    _locations.clear();

    _is_valid = false;

    delete[] _message;

    if (auto shared_shader = shader.lock()) {
        // TODO: Typecheck
        auto *gl_shader = reinterpret_cast<GLShader *>(shared_shader.get());
        glAttachShader(_program, gl_shader->get_vertex_shader());
        glAttachShader(_program, gl_shader->get_fragment_shader());
        glLinkProgram(_program);

        GLint status;
        glGetProgramiv(_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint log_length;
            GLint log_offset;
            GLsizei retrieved;
            glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &log_length);
            _message = new char[log_length];
            while (log_offset < log_length - 1) {
                glGetProgramInfoLog(_program, log_length - log_offset, &retrieved, _message + log_offset);
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
        glUseProgram(_program);
        for (auto& pair : _values) {
            GLint location = get_location(pair.first);
            _locations.insert(make_pair(pair.first, location));
            pair.second->do_gl_call(location);
        }
    }

}

template <>
UniformValue* GLMaterial::make_value<float>(float value) {
    return new FloatUniformValue(value);
}

template <>
UniformValue* GLMaterial::make_value<weak_ptr<Texture>>(
    weak_ptr<Texture> value
) {
    int index = _next_texture_index++;
    _textures.push_back(value);
    return new TextureUniformValue(value, index);
}

template <typename T>
void GLMaterial::set_uniform(const string &name, T value) {
    GLint location = get_cached_location(name);
    if (location == -1) {
        location = get_location(name);
        _locations.insert(make_pair(name, location));
    }
    
    auto it = _values.find(name);
    if (it == _values.end()) {
        _values.insert(make_pair(
            name, unique_ptr<UniformValue>(make_value<T>(value))
        ));
    }
    else {
        it->second = unique_ptr<UniformValue>(make_value<T>(value));
    }
}

template void GLMaterial::set_uniform<float>(const string &name, float value);
template void GLMaterial::set_uniform<weak_ptr<Texture>>(
    const string &name, weak_ptr<Texture> value
);

void GLMaterial::set_uniform_float(const std::string &name, float value) {
    set_uniform(name, value);
}

void GLMaterial::set_uniform_texture(
    const std::string &name, std::weak_ptr<Texture> value
) {
    set_uniform(name, value);
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

    if (auto shader = _shader.lock()) {
        // TODO: Ensure given shader is actually a GLShader
        GLShader *gl_shader = reinterpret_cast<GLShader*>(shader.get());
        return glGetUniformLocation(_program, name.c_str());
    }
    else {
        return -1;
    }
}

GLuint GLMaterial::get_program() const {
    return _program;
}

int GLMaterial::get_texture_count() const {
    return _textures.size();
}

weak_ptr<Texture> GLMaterial::get_texture(int i) const {
    return _textures[i];
}

// *****
// TODO: Move these into new files
// *****

FloatUniformValue::FloatUniformValue(float value) {
    _value = value;
}

void FloatUniformValue::do_gl_call(GLint location) {
    glUniform1f(location, _value);
}

TextureUniformValue::TextureUniformValue(weak_ptr<Texture> value, int index) {
    _value = value;
    _index = index;
}

void TextureUniformValue::do_gl_call(GLint location) {
    glUniform1i(location, _index);
}

