#include <giygas/GLFWContext.hpp>
#include <giygas/Renderer.hpp>
#include <giygas/giygas.hpp>
#include <giygasutil/GameLoopDelegate.hpp>
#include <giygasutil/GameLoopRunner.hpp>
#include <iostream>
#include <giygasutil/util.hpp>
#include "example_common.hpp"

using namespace giygas;
using namespace giygasutil;
using namespace std;
using namespace giygas_examples_common;

class VertexData {
public:
    Vector4 position;
    Vector4 color;
};

class TriangleExampleApp : public GameLoopDelegate {

    unique_ptr<Renderer> _renderer;
    ShaderLoader _shader_loader;
    unique_ptr<Shader> _vertex_shader;
    unique_ptr<Shader> _fragment_shader;
    unique_ptr<Pipeline> _pipeline;
    unique_ptr<VertexBuffer> _vertex_buffer;
    unique_ptr<IndexBuffer8> _index_buffer;
    unique_ptr<RenderPass> _pass;
    unique_ptr<CommandPool> _command_pool;
    unique_ptr<unique_ptr<CommandBuffer>[]> _command_buffers;
    unique_ptr<unique_ptr<Framebuffer>[]> _swapchain_framebuffers;

public:

    TriangleExampleApp(GLFWContext &context, const char *arg0) {
        _renderer = unique_ptr<Renderer>(giygas::make_renderer(&context));
        _shader_loader = ShaderLoader(get_content_dir(arg0).c_str(), _renderer->renderer_type());
    }

    void init() {
        _renderer->initialize();

        //
        // Setup mesh
        //
        array<VertexData, 3> verts = {
            VertexData{ Vector4(-1, 1, 0, 1), Vector4(1, 0, 0, 1) },
            VertexData{ Vector4(0, -1, 0, 1), Vector4(0, 1, 0, 1) },
            VertexData{ Vector4(1,  1, 0, 1), Vector4(0, 0, 1, 1) },
        };
        array<uint8_t, 3> indices = {0, 1, 2};

        _vertex_buffer = unique_ptr<VertexBuffer>(_renderer->make_vertex_buffer());
        VertexBuffer::set_data(*_vertex_buffer, 0, verts.data(), verts.size());

        _index_buffer = unique_ptr<IndexBuffer8>(_renderer->make_index_buffer_8());
        _index_buffer->set(0, indices.data(), indices.size());

        //
        // Setup Vertex Attribute layout
        //
        array<VertexAttribute, 2> vertex_attribs = {};
        VertexAttribute &position_attrib = vertex_attribs[0];
        VertexAttribute &color_attrib = vertex_attribs[1];
        position_attrib.component_count = 4;
        position_attrib.component_size = sizeof(VertexData::position);
        position_attrib.offset = offsetof(VertexData, position);
        color_attrib.component_count = 4;
        color_attrib.component_size = sizeof(VertexData::color);
        color_attrib.offset = offsetof(VertexData, color);
        VertexAttributeLayout layout = {};
        layout.stride = sizeof(VertexData);
        layout.attribute_count = vertex_attribs.size();
        layout.attributes = vertex_attribs.data();

        //
        // Setup shaders
        //
        size_t vertex_code_size, fragment_code_size;
        unique_ptr<uint8_t[]> vertex_code, fragment_code;
        vertex_code = _shader_loader.get_shader_code("shaders/triangle.vs", vertex_code_size);
        fragment_code = _shader_loader.get_shader_code("shaders/triangle.fs", fragment_code_size);
        _vertex_shader = unique_ptr<Shader>(_renderer->make_shader());
        _fragment_shader = unique_ptr<Shader>(_renderer->make_shader());
        _vertex_shader->set_code(vertex_code.get(), vertex_code_size, ShaderType::Vertex);
        _fragment_shader->set_code(fragment_code.get(), fragment_code_size, ShaderType::Fragment);

        //
        // Setup renderpass
        //
        RenderPassCreateParameters pass_params = {};
        RenderPassAttachment pass_attachment = {};
        pass_attachment.purpose = AttachmentPurpose::Color;
        pass_attachment.target = _renderer->get_swapchain_rendertarget(0);
        pass_params.attachment_count = 1;
        pass_params.attachments = &pass_attachment;
        _pass = unique_ptr<RenderPass>(_renderer->make_render_pass());
        _pass->create(pass_params);

        //
        // Create swapchain framebuffers
        //
        size_t framebuffer_count = _renderer->swapchain_framebuffer_count();
        _swapchain_framebuffers = unique_ptr<unique_ptr<Framebuffer>[]>(
            util::create_swapchain_framebuffers<unique_ptr<Framebuffer>>(
                _renderer.get(),
                _pass.get()
            )
        );

        //
        // Setup pipeline 
        //
        array<const Shader *, 2> shaders = {_vertex_shader.get(), _fragment_shader.get()};
        PipelineCreateParameters pipeline_params = {};
        pipeline_params.viewport.width = _renderer->swapchain_width();
        pipeline_params.viewport.height = _renderer->swapchain_height();
        pipeline_params.viewport.min_depth = 0;
        pipeline_params.viewport.min_depth = 1;
        pipeline_params.scissor.width = _renderer->swapchain_width();
        pipeline_params.scissor.height = _renderer->swapchain_height();
        pipeline_params.shader_count = 2;
        pipeline_params.shaders = shaders.data();
        pipeline_params.vertex_buffer_layout_count = 1;
        pipeline_params.vertex_buffer_layouts = &layout;
        pipeline_params.pass = _pass.get();
        _pipeline = unique_ptr<Pipeline>(_renderer->make_pipeline());
        _pipeline->create(pipeline_params);


        //
        // Setup command buffers
        //

        _command_pool = unique_ptr<CommandPool>(_renderer->make_command_pool());
        _command_pool->create();
        _command_buffers = unique_ptr<unique_ptr<CommandBuffer>[]>(
            new unique_ptr<CommandBuffer>[framebuffer_count]
        );

        ClearValue clear_value = {};
        clear_value.purpose = AttachmentPurpose::Color;
        clear_value.color_value = Vector4(0.5f, 0.5f, 1.0f, 1.0f);

        DrawInfo draw_info = {};
        const VertexBuffer *vertex_buffer = _vertex_buffer.get();
        const GenericIndexBuffer *index_buffer = _index_buffer.get();
        draw_info.pipeline = _pipeline.get();
        draw_info.vertex_buffer_count = 1;
        draw_info.vertex_buffers = &vertex_buffer;
        draw_info.index_buffer = _index_buffer.get();
        draw_info.index_range.offset = 0;
        draw_info.index_range.count = 3;

        for (uint32_t i = 0; i < framebuffer_count; ++i) {
            unique_ptr<CommandBuffer> &commands = _command_buffers[i];
            commands = unique_ptr<CommandBuffer>(_renderer->make_command_buffer());
            commands->create(_command_pool.get());

            SingleBufferPassInfo record_info = {};
            record_info.draw_count = 1;
            record_info.draws = &draw_info;
            record_info.pass_info.pass = _pass.get();
            record_info.pass_info.framebuffer = _swapchain_framebuffers[i].get();
            record_info.pass_info.clear_value_count = 1;
            record_info.pass_info.clear_values = &clear_value;

            commands->record_pass(record_info);
        }

    }

    void update_logic(float elapsed_seconds) override {
    }

    void update_graphics() override {
        const CommandBuffer *command_buffer
            = _command_buffers[_renderer->next_swapchain_framebuffer_index()].get();
        _renderer->submit(&command_buffer, 1);

        _renderer->present();
    }

};

int main (int argc, char **argv) {
    GLFWContext context;
    TriangleExampleApp app(context, argv[0]);
    GameLoopRunner runner(&context, &app);
    app.init();
    context.show();
    runner.run();
    return 0;
}
