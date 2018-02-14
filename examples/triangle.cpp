#include <giygas/GLFWContext.hpp>
#include <giygas/Renderer.hpp>
#include <giygas/giygas.hpp>
#include <giygasutil/EventLoopUpdatable.hpp>
#include <giygasutil/EventLoopContextRunner.hpp>
#include "example_common.hpp"

using namespace giygas;
using namespace std;
using namespace giygas_examples_common;

class TriangleExampleApp : public EventLoopUpdatable {

    unique_ptr<Renderer> _renderer;
    ShaderLoader _shader_loader;
    unique_ptr<Shader> _vertex_shader;
    unique_ptr<Shader> _fragment_shader;
    unique_ptr<RenderPass> _render_pass;
    unique_ptr<Pipeline> _pipeline;
    unique_ptr<VertexBuffer> _vertex_buffer;
    unique_ptr<IndexBuffer8> _index_buffer;
    unique_ptr<Material> _material;
    unique_ptr<CommandPool> _command_pool;
    unique_ptr<CommandBuffer[]> _command_buffers;

public:

    TriangleExampleApp(GLFWContext &context, const char *arg0) {
        _renderer = unique_ptr<Renderer>(giygas::make_renderer(&context));
        _shader_loader = ShaderLoader(arg0, _renderer->renderer_type());
    }

    void init() {
        _renderer->initialize();
        _vertex_shader = unique_ptr<Shader>(_renderer->make_shader());
        _fragment_shader = unique_ptr<Shader>(_renderer->make_shader());
        _render_pass = unique_ptr<RenderPass>(_renderer->make_renderpass());
        _pipeline = unique_ptr<Pipeline>(_renderer->make_pipeline());
        _command_pool = unique_ptr<CommandPool>(_renderer->make_commandpool());
        _vertex_buffer = unique_ptr<VertexBuffer>(_renderer->make_vertex_buffer());
        _index_buffer = unique_ptr<IndexBuffer8>(_renderer->make_index_buffer_8());
        _material = unique_ptr<Material>(_renderer->make_material());


        //
        // Setup shaders
        //
        size_t vertex_code_size, fragment_code_size;
        unique_ptr<uint8_t[]> vertex_code, fragment_code;
        vertex_code = _shader_loader.get_shader_code("shaders/color.vs", vertex_code_size);
        fragment_code = _shader_loader.get_shader_code("shaders/color.fs", fragment_code_size);
        _vertex_shader->set_code(vertex_code.get(), vertex_code_size, ShaderType::Vertex);
        _fragment_shader->set_code(fragment_code.get(), fragment_code_size, ShaderType::Fragment);

        //
        // Setup Vertex Attribute layout
        //
        VertexAttributeLayout layout(0, 0);

        //
        // Setup Render Pass
        //
        RenderPassCreateParameters pass_params = {};
        pass_params.color_attachment.api_format = 0; // TODO;
        _render_pass->create(pass_params);

        //
        // Setup pipeline 
        //
        array<const Shader *, 2> shaders = {_vertex_shader.get(), _fragment_shader.get()};
        PipelineCreateParameters pipeline_params = {};
        pipeline_params.shader_count = 2;
        pipeline_params.shaders = shaders.data();
        pipeline_params.vertex_buffer_layout_count = 1;
        pipeline_params.vertex_buffer_layouts = &layout;
        pipeline_params.render_pass = _render_pass.get();
        _pipeline->create(pipeline_params);

        //
        // Setup command buffers
        //
        uint32_t framebuffer_count = _renderer->swapchain_framebuffer_count();
        _command_buffers = unique_ptr<CommandBuffer[]>(new CommandBuffer[framebuffer_count]);
        for (uint32_t i = 0; i < framebuffer_count; ++i) {
            CommandBuffer &commands = _command_buffers[i];
            commands = _command_pool->take_static_buffer();
            DrawInfo draw_info = {};
            const VertexBuffer *vertex_buffer = _vertex_buffer.get();
            const GenericIndexBuffer *index_buffer = _index_buffer.get();
            draw_info.pipeline = _pipeline.get();
            draw_info.framebuffer = _renderer->get_swapchain_framebuffer(i);
            draw_info.vertex_buffer_count = 1;
            draw_info.vertex_buffers = &vertex_buffer;
            draw_info.index_buffer = _index_buffer.get();
            draw_info.pass = _render_pass.get();
            draw_info.index_range.offset = 0;
            draw_info.index_range.count = 3;
            draw_info.material = _material.get();
            commands.record(draw_info);
        }

    }

    void update(float elapsed_seconds) override {
        const CommandBuffer *command_buffer
            = &_command_buffers[_renderer->next_swapchain_framebuffer_index()];
        _renderer->submit(&command_buffer, 1);

        _renderer->present();
    }

};


int main (int argc, char **argv)
{
    GLFWContext context;
    TriangleExampleApp app(context, argv[0]);
    EventLoopContextRunner runner(&context, &app);
    app.init();
    context.show();
    runner.run();
    return 0;
}