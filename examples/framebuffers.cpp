#include <giygas/giygas.hpp>
#include <giygas/GLFWContext.hpp>
#include <giygasutil/GameLoopDelegate.hpp>
#include <giygasutil/GameLoopRunner.hpp>
#include <giygas/Matrix4x4.hpp>
#include <giygasutil/util.hpp>
#include "example_common.hpp"

using namespace giygas;
using namespace std;
using namespace giygas_examples_common;

class VertexData {
public:
    Vector3 position;
    Vector3 color;
    Vector2 uv;
};

class UniformData {
public:
    Matrix4x4 model_tf;
    Matrix4x4 view_tf;
    Matrix4x4 projection_tf;
};

class FramebufferExampleApp : public GameLoopDelegate  {

    unique_ptr<Renderer> _renderer;
    ShaderLoader _shader_loader;
    unique_ptr<VertexBuffer> _vertex_buffer;
    unique_ptr<IndexBuffer8> _index_buffer;
    unique_ptr<UniformBuffer> _offscreen_uniforms;
    unique_ptr<UniformBuffer> _onscreen_uniforms;
    unique_ptr<Sampler> _sampler;
    unique_ptr<DescriptorPool> _descriptor_pool;
    unique_ptr<DescriptorSet> _offscreen_descriptors;
    unique_ptr<DescriptorSet> _onscreen_descriptors;
    unique_ptr<Shader> _colored_shader_v;
    unique_ptr<Shader> _colored_shader_f;
    unique_ptr<Shader> _textured_shader_v;
    unique_ptr<Shader> _textured_shader_f;
    unique_ptr<Texture> _render_texture;
    unique_ptr<Texture> _render_depth_buffer;
    unique_ptr<RenderPass> _offscreen_pass;
    unique_ptr<RenderPass> _onscreen_pass;
    unique_ptr<Framebuffer> _offscreen_framebuffer;
    unique_ptr<Pipeline> _colored_pipeline;
    unique_ptr<Pipeline> _textured_pipeline;
    unique_ptr<CommandPool> _command_pool;
    unique_ptr<CommandBuffer> _offscreen_command_buffer;
    unique_ptr<CommandBuffer> _onscreen_command_buffer;
    unique_ptr<unique_ptr<Framebuffer>[]> _swapchain_framebuffers;

    float current_rotation;

public:

    FramebufferExampleApp(GLFWContext &context, const char *arg0) {
        _renderer = unique_ptr<Renderer>(giygas::make_renderer(&context));
        _shader_loader = ShaderLoader(get_content_dir(arg0).c_str(), _renderer->renderer_type());
        current_rotation = 0;
    }

    void setup() {
        _renderer->initialize();

        //
        // Setup mesh
        //
        array<VertexData, 8> vertices = {
            // pos, color, uv
            VertexData { Vector3(0, 0, 1), Vector3(0, 0, 1), Vector2(0, 0) },
            VertexData { Vector3(1, 0, 1), Vector3(1, 0, 1), Vector2(1, 0) },
            VertexData { Vector3(0, 0, 0), Vector3(0, 0, 0), Vector2(1, 0) },
            VertexData { Vector3(1, 0, 0), Vector3(1, 0, 0), Vector2(0, 0) },
            VertexData { Vector3(0, 1, 1), Vector3(0, 1, 1), Vector2(0, 1) },
            VertexData { Vector3(1, 1, 1), Vector3(1, 1, 1), Vector2(1, 1) },
            VertexData { Vector3(0, 1, 0), Vector3(0, 1, 0), Vector2(1, 1) },
            VertexData { Vector3(1, 1, 0), Vector3(1, 1, 0), Vector2(0, 1) }
        };

        array<uint8_t, 12 * 3> indices = {
            1, 0, 3, 3, 0, 2,  // bottom
            4, 5, 6, 6, 5, 7,  // top
            0, 1, 4, 4, 1, 5,  // front side
            3, 2, 7, 7, 2, 6,  // back side
            2, 0, 6, 6, 0, 4,  // left side
            1, 3, 5, 5, 3, 7,  // right side
        };

        _vertex_buffer = unique_ptr<VertexBuffer>(_renderer->make_vertex_buffer());
        VertexBuffer::set_data(*_vertex_buffer, 0, vertices.data(),vertices.size());

        _index_buffer = unique_ptr<IndexBuffer8>(_renderer->make_index_buffer_8());
        _index_buffer->set(0, indices.data(), indices.size());

        //
        // Setup Vertex Attribute layout
        //
        array<VertexAttribute, 3> layout_attribs = {};
        VertexAttribute &position_attrib = layout_attribs[0];
        VertexAttribute &color_attrib = layout_attribs[1];
        VertexAttribute &uv_attrib = layout_attribs[2];
        position_attrib.component_count = 3;
        position_attrib.component_size = sizeof(float);
        position_attrib.offset = offsetof(VertexData, position);
        color_attrib.component_count = 3;
        color_attrib.component_size = sizeof(float);
        color_attrib.offset = offsetof(VertexData, color);
        uv_attrib.component_count = 2;
        uv_attrib.component_size = sizeof(float);
        uv_attrib.offset = offsetof(VertexData, uv);
        VertexAttributeLayout layout = {};
        layout.stride = sizeof(VertexData);
        layout.attribute_count = layout_attribs.size();
        layout.attributes = layout_attribs.data();

        //
        // Setup uniform buffer
        //
        UniformData data = {};
        _onscreen_uniforms = unique_ptr<UniformBuffer>(_renderer->make_uniform_buffer());
        _offscreen_uniforms = unique_ptr<UniformBuffer>(_renderer->make_uniform_buffer());
        _onscreen_uniforms->set_data(0, reinterpret_cast<const uint8_t *>(&data), sizeof(data));
        _offscreen_uniforms->set_data(0, reinterpret_cast<const uint8_t *>(&data), sizeof(data));

        //
        // Create our textures and render buffers that we will render to
        //
        _render_texture = unique_ptr<Texture>(_renderer->make_texture());
        _render_depth_buffer = unique_ptr<Texture>(_renderer->make_texture());
        _render_texture->create(nullptr, 0, 512, 512, TextureFormat::RGBA, TextureFormat::RGBA, static_cast<TextureUsageFlags>(TEXTURE_USAGE_COLOR_ATTACHMENT | TEXTURE_USAGE_SAMPLE));
        _render_depth_buffer->create(nullptr, 0, 512, 512, TextureFormat::Depth16, TextureFormat::Depth16, TEXTURE_USAGE_DEPTH_ATTACHMENT);

        //
        // Setup sampler
        //
        SamplerParameters sampler_params = {};
        sampler_params.wrap_mode_u = SamplerWrapMode::Repeat;
        sampler_params.wrap_mode_v = SamplerWrapMode::Repeat;
        sampler_params.minify_filter_mode = SamplerFilterMode::Nearest;
        sampler_params.magnify_filter_mode = SamplerFilterMode::Nearest;
        sampler_params.mipmap_mode = SamplerMipmapMode::Nearest;
        _sampler = unique_ptr<Sampler>(_renderer->make_sampler());
        _sampler->create(sampler_params);

        //
        // Setup descriptor pool and set
        //
        DescriptorPoolParameters descriptor_pool_params = {};
        descriptor_pool_params.max_sets = 2;
        descriptor_pool_params.uniform_buffer_descriptors = 2;
        descriptor_pool_params.sampler_descriptors = 1;
        _descriptor_pool = unique_ptr<DescriptorPool>(_renderer->make_descriptor_pool());
        _descriptor_pool->create(descriptor_pool_params);

        DescriptorSetParameters offscreen_set_parameters = {};
        const UniformBuffer *offscreen_uniform_buffer = _offscreen_uniforms.get();
        offscreen_set_parameters.pool = _descriptor_pool.get();
        offscreen_set_parameters.uniform_buffer_count = 1;
        offscreen_set_parameters.uniform_buffers = &offscreen_uniform_buffer;
        _offscreen_descriptors = unique_ptr<DescriptorSet>(_renderer->make_descriptor_set());
        _offscreen_descriptors->create(offscreen_set_parameters);

        DescriptorSetParameters onscreen_set_parameters = {};
        const UniformBuffer *onscreen_uniform_buffer = _onscreen_uniforms.get();
        const Sampler *sampler = _sampler.get();
        const Texture *texture = _render_texture.get();
        onscreen_set_parameters.pool = _descriptor_pool.get();
        onscreen_set_parameters.uniform_buffer_count = 1;
        onscreen_set_parameters.uniform_buffers = &onscreen_uniform_buffer;
        onscreen_set_parameters.sampler_count = 1;
        onscreen_set_parameters.samplers = &sampler;
        onscreen_set_parameters.textures = &texture;
        _onscreen_descriptors = unique_ptr<DescriptorSet>(_renderer->make_descriptor_set());
        _onscreen_descriptors->create(onscreen_set_parameters);

        //
        // Setup shaders
        //
        size_t colored_v_size, colored_f_size, textured_v_size, textured_f_size;
        unique_ptr<uint8_t[]> colored_v_code, colored_f_code, textured_v_code, textured_f_code;
        colored_v_code = _shader_loader.get_shader_code("shaders/color.vs", colored_v_size);
        colored_f_code = _shader_loader.get_shader_code("shaders/color.fs", colored_f_size);
        textured_v_code = _shader_loader.get_shader_code("shaders/textured.vs", textured_v_size);
        textured_f_code = _shader_loader.get_shader_code("shaders/textured.fs", textured_f_size);
        _colored_shader_v = unique_ptr<Shader>(_renderer->make_shader());
        _colored_shader_f = unique_ptr<Shader>(_renderer->make_shader());
        _textured_shader_v = unique_ptr<Shader>(_renderer->make_shader());
        _textured_shader_f = unique_ptr<Shader>(_renderer->make_shader());
        _colored_shader_v->set_code(colored_v_code.get(), colored_v_size, ShaderType::Vertex);
        _colored_shader_f->set_code(colored_f_code.get(), colored_f_size, ShaderType::Fragment);
        _textured_shader_v->set_code(textured_v_code.get(), textured_v_size, ShaderType::Vertex);
        _textured_shader_f->set_code(textured_f_code.get(), textured_f_size, ShaderType::Fragment);

        //
        // Setup renderpasses
        //

        RenderPassCreateParameters offscreen_pass_params = {};
        array<RenderPassAttachment, 2> offscreen_attachments = {};
        offscreen_attachments[0].purpose = AttachmentPurpose::Color;
        offscreen_attachments[0].target = _render_texture.get();
        offscreen_attachments[1].purpose = AttachmentPurpose::DepthStencil;
        offscreen_attachments[1].target = _render_depth_buffer.get();
        offscreen_pass_params.attachment_count = offscreen_attachments.size();
        offscreen_pass_params.attachments = offscreen_attachments.data();
        _offscreen_pass = unique_ptr<RenderPass>(_renderer->make_render_pass());
        _offscreen_pass->create(offscreen_pass_params);

        RenderPassCreateParameters onscreen_pass_params = {};
        RenderPassAttachment onscreen_attachment = {};
        onscreen_attachment.purpose = AttachmentPurpose::Color;
        onscreen_attachment.target = _renderer->get_swapchain_rendertarget(0);
        onscreen_pass_params.attachment_count = 1;
        onscreen_pass_params.attachments = &onscreen_attachment;
        _onscreen_pass = unique_ptr<RenderPass>(_renderer->make_render_pass());
        _onscreen_pass->create(onscreen_pass_params);

        //
        // Setup offscreen framebuffer
        //
        FramebufferCreateParameters offscreen_fb_params = {};
        array<FramebufferAttachment, 2> fb_attachments = {};
        fb_attachments[0].purpose = AttachmentPurpose::Color;
        fb_attachments[0].target = _render_texture.get();
        fb_attachments[1].purpose = AttachmentPurpose::DepthStencil;
        fb_attachments[1].target = _render_depth_buffer.get();
        offscreen_fb_params.width = 512;
        offscreen_fb_params.height = 512;
        offscreen_fb_params.attachment_count = fb_attachments.size();
        offscreen_fb_params.attachments = fb_attachments.data();
        offscreen_fb_params.pass = _offscreen_pass.get();
        _offscreen_framebuffer = unique_ptr<Framebuffer>(_renderer->make_framebuffer());
        _offscreen_framebuffer->create(offscreen_fb_params);

        //
        // Create swapchain framebuffers
        //
        size_t framebuffer_count = _renderer->swapchain_framebuffer_count();
        _swapchain_framebuffers = unique_ptr<unique_ptr<Framebuffer>[]>(
            giygasutil::util::create_swapchain_framebuffers<unique_ptr<Framebuffer>>(
                _renderer.get(),
                _onscreen_pass.get()
            )
        );

        //
        // Setup pipelines
        //
        PipelineCreateParameters colored_pipeline_params = {};
        array<const Shader *, 2> colored_shaders = {_colored_shader_v.get(), _colored_shader_f.get()};
        colored_pipeline_params.viewport.width = 512;
        colored_pipeline_params.viewport.height = 512;
        colored_pipeline_params.viewport.min_depth = 0;
        colored_pipeline_params.viewport.min_depth = 1;
        colored_pipeline_params.scissor.width = 512;
        colored_pipeline_params.scissor.height = 512;
        colored_pipeline_params.shader_count = 2;
        colored_pipeline_params.shaders = colored_shaders.data();
        colored_pipeline_params.vertex_buffer_layout_count = 1;
        colored_pipeline_params.vertex_buffer_layouts = &layout;
        colored_pipeline_params.pass = _offscreen_pass.get();
        colored_pipeline_params.descriptor_set = _offscreen_descriptors.get();
        _colored_pipeline = unique_ptr<Pipeline>(_renderer->make_pipeline());
        _colored_pipeline->create(colored_pipeline_params);

        PipelineCreateParameters textured_pipeline_params = {};
        array<const Shader *, 2> textured_shaders = {_textured_shader_v.get(), _textured_shader_f.get()};
        textured_pipeline_params.viewport.width = _renderer->swapchain_width();
        textured_pipeline_params.viewport.height = _renderer->swapchain_height();
        textured_pipeline_params.viewport.min_depth = 0;
        textured_pipeline_params.viewport.min_depth = 1;
        textured_pipeline_params.scissor.width = _renderer->swapchain_width();
        textured_pipeline_params.scissor.height = _renderer->swapchain_height();
        textured_pipeline_params.shader_count = 2;
        textured_pipeline_params.shaders = textured_shaders.data();
        textured_pipeline_params.vertex_buffer_layout_count = 1;
        textured_pipeline_params.vertex_buffer_layouts = &layout;
        textured_pipeline_params.pass = _onscreen_pass.get();
        textured_pipeline_params.descriptor_set = _onscreen_descriptors.get();
        _textured_pipeline = unique_ptr<Pipeline>(_renderer->make_pipeline());
        _textured_pipeline->create(textured_pipeline_params);

        //
        // Make command buffers
        //
        _command_pool = unique_ptr<CommandPool>(_renderer->make_command_pool());
        _command_pool->create();
        _offscreen_command_buffer = unique_ptr<CommandBuffer>(_renderer->make_command_buffer());
        _onscreen_command_buffer = unique_ptr<CommandBuffer>(_renderer->make_command_buffer());
        _offscreen_command_buffer->create(_command_pool.get());
        _onscreen_command_buffer->create(_command_pool.get());

    }

    void update_logic(float elapsed_seconds) override {
        current_rotation += elapsed_seconds;
    }

    void update_graphics() override {
        //
        // Setup uniform data
        //
        UniformData offscreen_uniforms = {};
        float aspect = 512.0f / 512.0f;
        offscreen_uniforms.model_tf =
            Matrix4x4::scale(Vector4(1,1,1,1));
        offscreen_uniforms.view_tf =
            Matrix4x4::translate(Vector4(0, 0, -2, 1)) *
            Matrix4x4::rotation_y(current_rotation) *
            Matrix4x4::translate(Vector4(-0.5f, -0.5f, -0.5f, 1));
        offscreen_uniforms.projection_tf =
            Matrix4x4::perspective(aspect, 0.1f, 10.0f, 60.0f * (3.14159f / 180.0f));
        _offscreen_uniforms->set_data(0, reinterpret_cast<const uint8_t *>(&offscreen_uniforms), sizeof(offscreen_uniforms));

        UniformData onscreen_uniforms = {};
        aspect =
            static_cast<float>(_renderer->swapchain_width()) /
            static_cast<float>(_renderer->swapchain_height());
        onscreen_uniforms.model_tf =
            Matrix4x4::scale(Vector4(1,1,1,1));
        onscreen_uniforms.view_tf =
            Matrix4x4::translate(Vector4(0, 0, -2, 1)) *
            Matrix4x4::rotation_y(current_rotation) *
            Matrix4x4::translate(Vector4(-0.5f, -0.5f, -0.5f, 1));
        onscreen_uniforms.projection_tf =
            Matrix4x4::perspective(aspect, 0.1f, 10.0f, 60.0f * (3.14159f / 180.0f));
        _onscreen_uniforms->set_data(0, reinterpret_cast<const uint8_t *>(&onscreen_uniforms), sizeof(onscreen_uniforms));

        //
        // Reset and record command buffers
        //
        _command_pool->reset_buffers();

        const VertexBuffer *vertex_buffer = _vertex_buffer.get();

        array<ClearValue, 2> offscreen_clear_values = {};
        offscreen_clear_values[0].purpose = AttachmentPurpose::Color;
        offscreen_clear_values[0].color_value = Vector4(0.25f, 0.25f, 0.5f, 1.0f);
        offscreen_clear_values[1].purpose = AttachmentPurpose::DepthStencil;
        offscreen_clear_values[1].depth_stencil.depth = 0;
        offscreen_clear_values[1].depth_stencil.stencil = 0;

        DrawInfo offscreen_draw_info = {};
        offscreen_draw_info.pipeline = _colored_pipeline.get();
        offscreen_draw_info.vertex_buffer_count = 1;
        offscreen_draw_info.vertex_buffers = &vertex_buffer;
        offscreen_draw_info.index_buffer = _index_buffer.get();
        offscreen_draw_info.index_range.offset = 0;
        offscreen_draw_info.index_range.count = 12 * 3;
        offscreen_draw_info.descriptor_set = _offscreen_descriptors.get();

        SingleBufferPassInfo offscreen_record_info = {};
        offscreen_record_info.draw_count = 1;
        offscreen_record_info.draws = &offscreen_draw_info;
        offscreen_record_info.pass_info.pass = _offscreen_pass.get();
        offscreen_record_info.pass_info.framebuffer = _offscreen_framebuffer.get();
        offscreen_record_info.pass_info.clear_value_count = 2;
        offscreen_record_info.pass_info.clear_values = offscreen_clear_values.data();

        _offscreen_command_buffer->record_pass(offscreen_record_info);


        array<ClearValue, 2> onscreen_clear_values = {};
        onscreen_clear_values[0].purpose = AttachmentPurpose::Color;
        onscreen_clear_values[0].color_value = Vector4(0.5f, 0.5f, 1.0f, 1.0f);
        onscreen_clear_values[1].purpose = AttachmentPurpose::DepthStencil;
        onscreen_clear_values[1].depth_stencil.depth = 0;
        onscreen_clear_values[1].depth_stencil.stencil = 0;

        DrawInfo onscreen_cube_info = {};
        onscreen_cube_info.pipeline = _textured_pipeline.get();
        onscreen_cube_info.vertex_buffer_count = 1;
        onscreen_cube_info.vertex_buffers = &vertex_buffer;
        onscreen_cube_info.index_buffer = _index_buffer.get();
        onscreen_cube_info.index_range.offset = 0;
        onscreen_cube_info.index_range.count = 12 * 3;
        onscreen_cube_info.descriptor_set = _onscreen_descriptors.get();

        SingleBufferPassInfo onscreen_record_info = {};
        onscreen_record_info.draw_count = 1;
        onscreen_record_info.draws = &onscreen_cube_info;
        onscreen_record_info.pass_info.pass = _onscreen_pass.get();
        onscreen_record_info.pass_info.framebuffer = _swapchain_framebuffers[_renderer->next_swapchain_framebuffer_index()].get();
        onscreen_record_info.pass_info.clear_value_count = 2;
        onscreen_record_info.pass_info.clear_values = onscreen_clear_values.data();

        _onscreen_command_buffer->record_pass(onscreen_record_info);

        //
        // Submit buffers
        //

        array<const CommandBuffer *, 2> buffers_to_submit = {
            _offscreen_command_buffer.get(), _onscreen_command_buffer.get()
        };
        _renderer->submit(buffers_to_submit.data(), buffers_to_submit.size());

        _renderer->present();
    }

};

int main(int argc, char **argv) {
    GLFWContext context;
    FramebufferExampleApp app(context, argv[0]);
    GameLoopRunner runner(&context, &app);
    app.setup();
    context.show();
    runner.run();
    return 0;
}
