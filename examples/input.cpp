#include <giygas/GLFWContext.hpp>
#include <giygas/Renderer.hpp>
#include <giygas/giygas.hpp>
#include <giygasutil/GameLoopDelegate.hpp>
#include <giygasutil/GameLoopRunner.hpp>
#include <giygasutil/util.hpp>
#include "example_common.hpp"
#include <giygas/Matrix4x4.hpp>
#include <array>

using namespace giygas;
using namespace giygasutil;
using namespace std;
using namespace giygas_examples_common;

class VertexData {
public:
    Vector2 position;
};

class PushConstantData {
public:
    Matrix4x4 tf;
};

class TriangleExampleApp : public GameLoopDelegate {
    Context *_context;
    unique_ptr<Renderer> _renderer;
    ShaderLoader _shader_loader;
    unique_ptr<VertexBuffer> _vertex_buffer;
    unique_ptr<IndexBuffer8> _index_buffer;
    unique_ptr<Shader> _vs;
    unique_ptr<Shader> _fs;
    unique_ptr<RenderPass> _pass;
    unique_ptr<Pipeline> _pipeline;
    unique_ptr<CommandPool> _command_pool;
    unique_ptr<CommandBuffer> _commands;
    unique_ptr<unique_ptr<Framebuffer>[]> _swapchain_framebuffers;

    EventHandler<unsigned int, float> _input_changed_handler;
    bool _is_up_pressed;
    bool _is_down_pressed;
    bool _is_left_pressed;
    bool _is_right_pressed;
    Vector2 _position;

public:

    TriangleExampleApp(GLFWContext *context, const char *arg0) {
        _context = context;
        _renderer = unique_ptr<Renderer>(giygas::make_renderer(context));
        _shader_loader = ShaderLoader(get_content_dir(arg0).c_str(), _renderer->renderer_type());
        _input_changed_handler = context->input_changed();
        _input_changed_handler.delegate = bind(
            &TriangleExampleApp::handle_input_changed,
            this, placeholders::_1, placeholders::_2
        );

        _is_up_pressed = false;
        _is_down_pressed = false;
        _is_left_pressed = false;
        _is_right_pressed = false;
        _position = {};
    }

    void init() {
        _renderer->initialize();

        //
        // Setup mesh
        //
        array<VertexData, 8> vertices = {
            VertexData { Vector2(0, 0) },
            VertexData { Vector2(1, 0) },
            VertexData { Vector2(0, 1) },
            VertexData { Vector2(1, 1) }
        };

        array<uint8_t, 6> indices = {0, 2, 1, 1, 2, 3};

        _vertex_buffer = unique_ptr<VertexBuffer>(_renderer->make_vertex_buffer());
        VertexBuffer::set_data(*_vertex_buffer, 0, vertices.data(),vertices.size());

        _index_buffer = unique_ptr<IndexBuffer8>(_renderer->make_index_buffer_8());
        _index_buffer->set(0, indices.data(), indices.size());

        //
        // Setup Vertex Attribute layout
        //
        array<VertexAttribute, 1> layout_attribs = {};
        VertexAttribute &position_attrib = layout_attribs[0];
        position_attrib.component_count = 2;
        position_attrib.component_size = sizeof(float);
        position_attrib.offset = offsetof(VertexData, position);
        VertexAttributeLayout layout = {};
        layout.stride = sizeof(VertexData);
        layout.attribute_count = layout_attribs.size();
        layout.attributes = layout_attribs.data();

        //
        // Setup shaders
        //
        size_t vs_size, fs_size;
        unique_ptr<uint8_t[]> vs_code, fs_code;
        vs_code = _shader_loader.get_shader_code("shaders/input_example.vs", vs_size);
        fs_code = _shader_loader.get_shader_code("shaders/input_example.fs", fs_size);
        _vs = unique_ptr<Shader>(_renderer->make_shader());
        _fs = unique_ptr<Shader>(_renderer->make_shader());
        _vs->set_code(vs_code.get(), vs_size, ShaderType::Vertex);
        _fs->set_code(fs_code.get(), fs_size, ShaderType::Fragment);

        //
        // Setup renderpasses
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
            giygasutil::util::create_swapchain_framebuffers<unique_ptr<Framebuffer>>(
                _renderer.get(),
                _pass.get()
            )
        );

        //
        // Setup pipelines
        //
        PipelineCreateParameters pipeline_params = {};
        array<const Shader *, 2> textured_shaders = {_vs.get(), _fs.get()};
        pipeline_params.viewport.width = _renderer->swapchain_width();
        pipeline_params.viewport.height = _renderer->swapchain_height();
        pipeline_params.viewport.min_depth = 0;
        pipeline_params.viewport.min_depth = 1;
        pipeline_params.scissor.width = _renderer->swapchain_width();
        pipeline_params.scissor.height = _renderer->swapchain_height();
        pipeline_params.shader_count = 2;
        pipeline_params.shaders = textured_shaders.data();
        pipeline_params.vertex_buffer_layout_count = 1;
        pipeline_params.vertex_buffer_layouts = &layout;
        pipeline_params.pass = _pass.get();
        pipeline_params.vertex_push_constants.size = sizeof(PushConstantData);
        _pipeline = unique_ptr<Pipeline>(_renderer->make_pipeline());
        _pipeline->create(pipeline_params);

        //
        // Make command buffers
        //
        _command_pool = unique_ptr<CommandPool>(_renderer->make_command_pool());
        _command_pool->create();
        _commands = unique_ptr<CommandBuffer>(_renderer->make_command_buffer());
        _commands->create(_command_pool.get());
    }

    void update_logic(float elapsed_seconds) override {

        Vector2 direction = {};
        if (_is_left_pressed) {
            direction.x = -1;
        }
        if (_is_right_pressed) {
            direction.x = 1;
        }
        if (_is_up_pressed) {
            direction.y = 1;
        }
        if (_is_down_pressed) {
            direction.y = -1;
        }

        _position += direction * elapsed_seconds;
    }

    void update_graphics() override {

        //
        // Setup push constant data
        //
        PushConstantData push_data = {};
        float aspect = (float)_renderer->swapchain_width() / (float)_renderer->swapchain_height();
        const float playfield_width = 10;
        const float playfield_height = 10;
        Matrix4x4 projection = Matrix4x4::scale(Vector4((1.0f / aspect) / playfield_width, 1.0f / playfield_height, 1.0f, 1.0f));
        push_data.tf = projection * Matrix4x4::translate(Vector4(_position.x, _position.y, 0, 1));

        //
        // Reset and record command buffers
        //
        _command_pool->reset_buffers();

        const VertexBuffer *vertex_buffer = _vertex_buffer.get();

        ClearValue clear_value;
        clear_value.purpose = AttachmentPurpose::Color;
        clear_value.color_value = Vector4(0.5f, 0.5f, 1.0f, 1.0f);

        DrawInfo draw_info = {};
        draw_info.pipeline = _pipeline.get();
        draw_info.vertex_buffer_count = 1;
        draw_info.vertex_buffers = &vertex_buffer;
        draw_info.index_buffer = _index_buffer.get();
        draw_info.index_range.offset = 0;
        draw_info.index_range.count = 6;
        draw_info.vertex_push_constants.range.offset = 0;
        draw_info.vertex_push_constants.range.size = sizeof(PushConstantData);
        draw_info.vertex_push_constants.data = reinterpret_cast<const uint8_t *>(&push_data);

        SingleBufferPassInfo onscreen_record_info = {};
        onscreen_record_info.draw_count = 1;
        onscreen_record_info.draws = &draw_info;
        onscreen_record_info.pass_info.pass = _pass.get();
        onscreen_record_info.pass_info.framebuffer = _swapchain_framebuffers[_renderer->next_swapchain_framebuffer_index()].get();
        onscreen_record_info.pass_info.clear_value_count = 1;
        onscreen_record_info.pass_info.clear_values = &clear_value;

        _commands->record_pass(onscreen_record_info);

        //
        // Submit buffers
        //
        const CommandBuffer *commands = _commands.get();
        _renderer->submit(&commands, 1);

        _renderer->present();
    }

    void handle_input_changed(unsigned int id, float value) {
        bool state = value > 0;

        if (id == _context->translate_key(InputKey::KEYBOARD_W)) {
            _is_up_pressed = state;
        }
        if (id == _context->translate_key(InputKey::KEYBOARD_S)) {
            _is_down_pressed = state;
        }
        if (id == _context->translate_key(InputKey::KEYBOARD_A)) {
            _is_left_pressed = state;
        }
        if (id == _context->translate_key(InputKey::KEYBOARD_D)) {
            _is_right_pressed = state;
        }
    }

    bool should_close() const override {
        return false;
    }

};

int main (int argc, char **argv) {
    GLFWContext context;
    TriangleExampleApp app(&context, argv[0]);
    GameLoopRunner runner(&context, &app);
    app.init();
    context.show();
    runner.run();
    return 0;
}
