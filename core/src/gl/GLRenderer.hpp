#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Primitive.hpp>
#include "giygas/Context.hpp"
#include "GLImpl.hpp"
#include "GLMainSurface.hpp"
#include "operations/GenBufferGLOperation.hpp"
#include "GLOperation.hpp"
#include "Pool.hpp"
#include "GLOperationPools.hpp"
#include <mutex>
#include <queue>
#include <thread>

namespace giygas {
    using namespace std;


    class QueuedGLOperation {
    public:
        GLOperation *op;
        TypelessPool *pool;
    };

    class GLRenderer : public Renderer {
        GLImpl _gl;
        GLMainSurface _main_surface;
        shared_ptr<Context> _context;
        queue<QueuedGLOperation> _operations_a;
        queue<QueuedGLOperation> _operations_b;
        queue<QueuedGLOperation> *_current_operation_queue;
        queue<QueuedGLOperation> *_next_operation_queue;
        mutex _swap_operations_mutex;
        condition_variable _worker_cv;
        thread _worker_thread;
        bool _worker_should_be_running;
        GLOperationPools _pools;
        EventHandler<unsigned int, unsigned int> _surface_size_changed_handler;

#ifndef NDEBUG
        bool _initialized;
#endif

        void move_common(GLRenderer &&other) noexcept;

        void handle_surface_size_changed(
            unsigned int width,
            unsigned int height
        );

        void start_worker();
        void stop_worker();
        void worker_loop();
        bool worker_routine();

    public:
        GLRenderer(shared_ptr<Context> window);
        GLRenderer(const GLRenderer &) = delete;
        GLRenderer(GLRenderer &&) noexcept;
        GLRenderer& operator=(const GLRenderer &) = delete;
        GLRenderer& operator=(GLRenderer &&) noexcept;
        ~GLRenderer();

        void initialize(/* PipelineOptions options */) override;

//        void set_polygon_culling_enabled(bool value) override;
//        void set_polygon_culling_mode(PolygonCullingMode value) override;
//        void set_front_face_winding(VertexWinding value) override;
//
//        void set_depth_test_enabled(bool value) override;
//        void set_depth_mask_enabled(bool value) override;
//        void set_depth_function(DepthFunction value) override;
//        void set_depth_range(double near, double far) override;

        VertexBuffer *make_vbo() override;
        ElementBuffer<unsigned int> *make_int_ebo() override;
        ElementBuffer<unsigned short> *make_short_ebo() override;
        ElementBuffer<unsigned char> *make_char_ebo() override;
        VertexArray *make_vao() override;
        Material *make_material() override;
        Shader *make_shader() override;
        Texture *make_texture(TextureInitOptions options) override;
        FrameBufferSurface *make_framebuffer() override;
        RenderBuffer *make_renderbuffer() override;
        Surface *main_surface() override;

        void present() override;

        void add_operation(GLOperation *op, TypelessPool *pool);
        void add_operation_and_notify(GLOperation *op, TypelessPool *pool);
        //void add_operation_delete_buffer(GLuint handle);

        GLOperationPools &pools();
    };

}
