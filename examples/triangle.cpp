#include <giygas/GLFWContext.hpp>
#include <giygas/Renderer.hpp>
#include <giygas/giygas.hpp>
#include <giygasutil/EventLoopUpdatable.hpp>
#include <giygasutil/EventLoopContextRunner.hpp>
#include <iostream>
#include "example_common.hpp"

using namespace giygas;
using namespace std;
using namespace giygas_examples_common;

class VertexData {
public:
    Vector4 position;
    Vector4 color;
};

class TriangleExampleApp : public EventLoopUpdatable {

    unique_ptr<Renderer> _renderer;
    ShaderLoader _shader_loader;
    unique_ptr<Shader> _vertex_shader;
    unique_ptr<Shader> _fragment_shader;
    //unique_ptr<RenderPass> _render_pass;
    unique_ptr<Pipeline> _pipeline;
    unique_ptr<VertexBuffer> _vertex_buffer;
    unique_ptr<IndexBuffer8> _index_buffer;
    unique_ptr<CommandPool> _command_pool;
    unique_ptr<unique_ptr<CommandBuffer>[]> _command_buffers;
    unique_ptr<unique_ptr<Framebuffer>[]> _swapchain_framebuffers;

public:

    TriangleExampleApp(GLFWContext &context, const char *arg0) {
        _renderer = unique_ptr<Renderer>(giygas::make_renderer(&context));
        _shader_loader = ShaderLoader(arg0, _renderer->renderer_type());
    }

    void init() {
        _renderer->initialize();
        _vertex_buffer = unique_ptr<VertexBuffer>(_renderer->make_vertex_buffer());
        _index_buffer = unique_ptr<IndexBuffer8>(_renderer->make_index_buffer_8());
        _vertex_shader = unique_ptr<Shader>(_renderer->make_shader());
        _fragment_shader = unique_ptr<Shader>(_renderer->make_shader());
        //_render_pass = unique_ptr<RenderPass>(_renderer->make_renderpass());
        _pipeline = unique_ptr<Pipeline>(_renderer->make_pipeline());
        _command_pool = unique_ptr<CommandPool>(_renderer->make_commandpool());
        _vertex_buffer = unique_ptr<VertexBuffer>(_renderer->make_vertex_buffer());
        _index_buffer = unique_ptr<IndexBuffer8>(_renderer->make_index_buffer_8());

        //
        // Setup mesh
        //
        array<VertexData, 3> verts = {
            VertexData{ Vector4(-1, 1, 0, 1), Vector4(1, 0, 0, 1) },
            VertexData{ Vector4(0, -1, 0, 1), Vector4(0, 1, 0, 1) },
            VertexData{ Vector4(1,  1, 0, 1), Vector4(0, 0, 1, 1) },
        };
        array<uint8_t, 3> indices = {0, 1, 2};

        VertexBuffer::set_data(*_vertex_buffer, 0, verts.data(), verts.size());
        _index_buffer->set(0, indices.data(), indices.size());

        //
        // Setup Vertex Attribute layout
        //
        VertexAttributeLayout layout(0, sizeof(VertexData));
        LayoutAttribute position_attrib = {};
        position_attrib.component_count = 4;
        position_attrib.component_size = sizeof(VertexData::position);
        position_attrib.offset = offsetof(VertexData, position);
        LayoutAttribute color_attrib = {};
        color_attrib.component_count = 4;
        color_attrib.component_size = sizeof(VertexData::color);
        color_attrib.offset = offsetof(VertexData, color);
        layout.add_attribute(position_attrib);
        layout.add_attribute(color_attrib);

        //
        // Setup shaders
        //
        size_t vertex_code_size, fragment_code_size;
        unique_ptr<uint8_t[]> vertex_code, fragment_code;
        vertex_code = _shader_loader.get_shader_code("shaders/triangle.vs", vertex_code_size);
        fragment_code = _shader_loader.get_shader_code("shaders/triangle.fs", fragment_code_size);
        _vertex_shader->set_code(vertex_code.get(), vertex_code_size, ShaderType::Vertex);
        _fragment_shader->set_code(fragment_code.get(), fragment_code_size, ShaderType::Fragment);

        //
        // Setup Render Pass
        //
//        RenderPassCreateParameters pass_params = {};
//        pass_params.color_attachment.api_format = _renderer->swapchain_api_format();
//        _render_pass->create(pass_params);


        //
        // Create swapchain framebuffers
        //
        uint32_t framebuffer_count = _renderer->swapchain_framebuffer_count();
        _swapchain_framebuffers = unique_ptr<unique_ptr<Framebuffer>[]>(
            new unique_ptr<Framebuffer>[framebuffer_count]
        );
        for (uint32_t i = 0; i < framebuffer_count; ++i) {
            unique_ptr<Framebuffer> &framebuffer = _swapchain_framebuffers[i];

            FramebufferCreateParameters fb_params = {};
            //fb_params.pass = _render_pass.get();
            fb_params.width = _renderer->swapchain_width();
            fb_params.height = _renderer->swapchain_height();
            fb_params.attachment_count = 1;
            FramebufferAttachment attachment = {};
            attachment.purpose = AttachmentPurpose::Color;
            attachment.target = _renderer->get_swapchain_rendertarget(i);
            fb_params.attachments = &attachment;
            framebuffer = unique_ptr<Framebuffer>(_renderer->make_framebuffer());
            framebuffer->create(fb_params);
        }

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
        pipeline_params.framebuffer = _swapchain_framebuffers[0].get();
        //pipeline_params.render_pass = _render_pass.get();
        _pipeline->create(pipeline_params);


        ClearValue clear_value = {};
        clear_value.purpose = AttachmentPurpose::Color;
        clear_value.color_value = Vector4(0.5f, 0.5f, 1.0f, 1.0f);

        //
        // Setup command buffers
        //
        _command_buffers = unique_ptr<unique_ptr<CommandBuffer>[]>(
            new unique_ptr<CommandBuffer>[framebuffer_count]
        );
        for (uint32_t i = 0; i < framebuffer_count; ++i) {
            unique_ptr<CommandBuffer> &commands = _command_buffers[i];
            commands = unique_ptr<CommandBuffer>(_command_pool->make_buffer());
            DrawInfo draw_info = {};
            const VertexBuffer *vertex_buffer = _vertex_buffer.get();
            const GenericIndexBuffer *index_buffer = _index_buffer.get();
            draw_info.pipeline = _pipeline.get();
            draw_info.framebuffer = _swapchain_framebuffers[i].get();
            draw_info.vertex_buffer_count = 1;
            draw_info.vertex_buffers = &vertex_buffer;
            draw_info.index_buffer = _index_buffer.get();
            //draw_info.pass = _render_pass.get();
            draw_info.index_range.offset = 0;
            draw_info.index_range.count = 3;
            draw_info.clear_values = &clear_value;
            commands->record(draw_info);
        }

    }

    void update(float elapsed_seconds) override {
        //cout << "UPDATE" << endl;

        const CommandBuffer *command_buffer
            = _command_buffers[_renderer->next_swapchain_framebuffer_index()].get();
        _renderer->submit(&command_buffer, 1);

        _renderer->present();
    }

};


int main (int argc, char **argv) {
    GLFWContext context;
    TriangleExampleApp app(context, argv[0]);
    EventLoopContextRunner runner(&context, &app);
    app.init();
    context.show();
    runner.run();
    return 0;
}