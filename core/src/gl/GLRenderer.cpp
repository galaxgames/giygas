#include <cassert>
#include <glad/glad.h>
#include <giygas/GLContext.hpp>
#include "GLVertexArray.hpp"
#include "GLRenderer.hpp"
#include "GLVertexBuffer.hpp"
#include "GLElementBuffer.hpp"
#include "GLMaterial.hpp"
#include "GLTexture.hpp"
#include "GLFrameBufferSurface.hpp"
#include "GLRenderBuffer.hpp"
#include "operations/PresentGLOperation.hpp"

using namespace std;
using namespace giygas;

GLRenderer::GLRenderer(shared_ptr<Context> window)
    : _main_surface(this)
    , _context(move(window))
{
#ifndef NDEBUG
    _initialized = false;
#endif

    _current_operation_queue = &_operations_a;
    _next_operation_queue = &_operations_b;
}

GLRenderer::GLRenderer(GLRenderer &&other) noexcept
    : _surface_size_changed_handler(move(other._surface_size_changed_handler))
{
    move_common(move(other));
}

GLRenderer& GLRenderer::operator=(GLRenderer &&other) noexcept {
    if (&other == this) {
        return *this;
    }
    _surface_size_changed_handler = move(other._surface_size_changed_handler);
    move_common(move(other));
    return *this;
}

void GLRenderer::move_common(GLRenderer &&other) noexcept {
    bool was_other_worker_running = other._worker_should_be_running;
    other.stop_worker();

    _gl = move(other._gl);
    _main_surface = move(other._main_surface);
    _context = move(other._context);
    _operations_a = move(other._operations_a);
    _operations_b = move(other._operations_b);
    _current_operation_queue = other._current_operation_queue;
    _next_operation_queue = other._next_operation_queue;

#ifndef NDEBUG
    _initialized = other._initialized;
    other._initialized = false;
#endif

    if (was_other_worker_running) {
        start_worker();
    }
}

GLRenderer::~GLRenderer() {
    stop_worker();
}

void GLRenderer::initialize(/* PipelineOptions options */) {
    assert(_context->is_valid());

    // TODO: Stop using glad so we can support having more than one GLRenderer
    // instance
    gladLoadGL();
    _surface_size_changed_handler = _context->surface_size_changed();
    _surface_size_changed_handler.delegate = bind(
        &GLRenderer::handle_surface_size_changed,
        this,
        placeholders::_1,
        placeholders::_2
    );
    _main_surface.set_size(
        _context->framebuffer_width(),
        _context->framebuffer_height()
    );

    #ifndef NDEBUG
        _initialized = true;
    #endif

//    set_polygon_culling_enabled(options.polygon_culling.enabled);
//    set_polygon_culling_mode(options.polygon_culling.mode);
//    set_front_face_winding(options.polygon_culling.front_face_vertex_winding);
//
//    set_depth_test_enabled(options.depth_buffer.depth_test_enabled);
//    set_depth_mask_enabled(options.depth_buffer.mask_enabled);
//    set_depth_function(options.depth_buffer.function);
//    set_depth_range(
//        options.depth_buffer.range_near,
//        options.depth_buffer.range_far
//    );

    start_worker();
}

void GLRenderer::start_worker() {
    _worker_should_be_running = true;
    _worker_thread = thread(&GLRenderer::worker_loop, this);
}

void GLRenderer::stop_worker() {
    {
        lock_guard<mutex> lock(_swap_operations_mutex);
        _worker_should_be_running = false;
        _worker_cv.notify_all();
    }
    _worker_thread.join();
}

//void GLRenderer::set_polygon_culling_enabled(bool value) {
//    assert(_initialized);
//    Pool<PipelineEnableDisableGLOperation> &pool = _pools._enable_disable_ops;
//    PipelineEnableDisableGLOperation *op = pool.take();
//    op->set(GL_CULL_FACE, value);
//    add_operation(op, &pool);
//}
//
//void GLRenderer::set_polygon_culling_mode(PolygonCullingMode value) {
//    assert(_initialized);
//    Pool<PipelineSetCullFaceGLOperation> &pool = _pools._set_cull_face_ops;
//    PipelineSetCullFaceGLOperation *op = pool.take();
//    op->set(value == PolygonCullingMode::BackFace ? GL_BACK : GL_FRONT);
//    add_operation(op, &pool);
//}
//
//void GLRenderer::set_front_face_winding(VertexWinding value) {
//    assert(_initialized);
//    Pool<PipelineSetFrontFaceGLOperation> &pool = _pools._set_front_face_ops;
//    PipelineSetFrontFaceGLOperation *op = pool.take();
//    op->set(value == VertexWinding::CounterClockwise ? GL_CCW : GL_CW);
//    add_operation(op, &pool);
//}
//
//void GLRenderer::set_depth_test_enabled(bool value) {
//    assert(_initialized);
//    Pool<PipelineEnableDisableGLOperation> &pool = _pools._enable_disable_ops;
//    PipelineEnableDisableGLOperation *op = pool.take();
//    op->set(GL_DEPTH_TEST, value);
//    add_operation(op, &pool);
//}
//
//void GLRenderer::set_depth_mask_enabled(bool value) {
//    assert(_initialized);
//    glDepthMask(static_cast<GLboolean>(value));
//}
//
//void GLRenderer::set_depth_function(DepthFunction value) {
//    assert(_initialized);
//    glDepthFunc(depth_function_to_enum(value));
//}
//
//void GLRenderer::set_depth_range(double near, double far) {
//    assert(_initialized);
//    glDepthRange(near, far);
//}

VertexBuffer *GLRenderer::make_vbo() {
    assert(_initialized);
    return new GLVertexBuffer(this);
}

VertexArray *GLRenderer::make_vao() {
    assert(_initialized);
    return new GLVertexArray(this);
}

IndexBuffer<unsigned int> *GLRenderer::make_ebo32() {
    assert(_initialized);
    return new GLElementBuffer<uint32_t>(this);
}

IndexBuffer<unsigned short> *GLRenderer::make_ebo16() {
    assert(_initialized);
    return new GLElementBuffer<uint16_t>(this);
}

IndexBuffer<unsigned char> *GLRenderer::make_ebo8() {
    assert(_initialized);
    return new GLElementBuffer<uint8_t>(this);
}

Material *GLRenderer::make_material() {
    assert(_initialized);
    return new GLMaterial(this);
}

Shader *GLRenderer::make_shader() {
    assert(_initialized);
    return new GLShader(this);
}

Texture *GLRenderer::make_texture(TextureInitOptions options) {
    assert(_initialized);
    return new GLTexture(this, options);
}

FrameBufferSurface *GLRenderer::make_framebuffer() {
    assert(_initialized);
    return new GLFrameBufferSurface(this);
}

RenderBuffer *GLRenderer::make_renderbuffer() {
    assert(_initialized);
    return new GLRenderBuffer(this);
}

Surface *GLRenderer::main_surface() {
    return &_main_surface;
}

void GLRenderer::present() {
    void *specific_context = _context->cast_to_specific(RendererType::OpenGL);
    GLContext *gl_context = static_cast<GLContext *>(specific_context);
    PresentGLOperation op(gl_context);
    add_operation_and_notify(&op, nullptr);
    op.wait();
}

void GLRenderer::handle_surface_size_changed(
    unsigned int width,
    unsigned int height
) {
    _main_surface.set_size(width, height);
}

void GLRenderer::add_operation(GLOperation *op, TypelessPool *pool) {
    lock_guard<mutex> lock(_swap_operations_mutex);
    QueuedGLOperation queued_op;
    queued_op.op = op;
    queued_op.pool = pool;
    _next_operation_queue->push(queued_op);
}

void GLRenderer::add_operation_and_notify(
    GLOperation *op,
    TypelessPool *pool
) {
    lock_guard<mutex> lock(_swap_operations_mutex);
    QueuedGLOperation queued_op;
    queued_op.op = op;
    queued_op.pool = pool;
    _next_operation_queue->push(queued_op);
    _worker_cv.notify_all();
}

GLOperationPools& GLRenderer::pools() {
    return _pools;
}

void GLRenderer::worker_loop() {
    void *specific_context = _context->cast_to_specific(RendererType::OpenGL);
    GLContext *gl_context = static_cast<GLContext *>(specific_context);
    gl_context->make_current_on_calling_thread();

    while (worker_routine()) { }
}

bool GLRenderer::worker_routine() {

    unique_lock<mutex> swap_lock(_swap_operations_mutex);
    _worker_cv.wait(swap_lock, [this]{
        return _current_operation_queue->empty() || !_worker_should_be_running;
    });
    swap(_current_operation_queue, _next_operation_queue);
    if (_current_operation_queue->empty() && !_worker_should_be_running) {
        return false;
    }
    swap_lock.unlock();

    while (!_current_operation_queue->empty())  {
        QueuedGLOperation op_info = _current_operation_queue->front();
        _current_operation_queue->pop();
        GLOperation *op = op_info.op;
        TypelessPool *pool = op_info.pool;
        op->execute(&_gl);
        if (pool != nullptr) {
            op_info.pool->give(op);
        }
    }

    return true;
}

