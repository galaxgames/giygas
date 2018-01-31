#include <giygas/giygas.hpp>
#include <giygas/GLFWContext.hpp>
#include <iostream>
#include <giygasutil/EventLoopUpdatable.hpp>
#include <giygasutil/EventLoopContextRunner.hpp>

using namespace giygas;
using namespace std;

class FramebufferExample : public EventLoopUpdatable
{
public:
    Renderer *renderer;
    unique_ptr<VertexBuffer> vbo;
    unique_ptr<IndexBuffer8> ebo;
    unique_ptr<VertexArray> vao;
    unique_ptr<Material> colored_material;
    unique_ptr<Material> textured_material;
    unique_ptr<Shader> colored_shader_v;
    unique_ptr<Shader> colored_shader_f;
    unique_ptr<Shader> textured_shader_v;
    unique_ptr<Shader> textured_shader_f;
    unique_ptr<FrameBufferSurface> framebuffer;
    shared_ptr<Texture> render_texture;
    unique_ptr<RenderBuffer> render_depth_buffer;

    float current_rotation;

    explicit FramebufferExample(Renderer *renderer)
        : renderer(renderer)
    {
        current_rotation = 0;
    }

    void setup_shader(
            Shader &shader,
            const char *vertex_source,
            const char *fragment_source,
            const char *shader_name
    ) {
//        shader.set_from_source(vertex_source, fragment_source);
//        if (!shader.is_valid()) {
//            cout << "Shader \"" << shader_name << "\" is invalid." << endl
//                 << "Vertex message: " << shader.get_vertex_message() << endl
//                 << "Fragment message: " << shader.get_fragment_message() << endl;
//        }
    }

    void setup_colored_shader(Shader *vs, Shader *fs) {
//        const char *vertex_source =
//            "#version 330\n"
//            "layout(location = 0)in vec3 pos;\n"
//            "layout(location = 2)in vec3 vertexColor;\n"
//            "out vec4 color;\n"
//            "uniform mat4 modelView;\n"
//            "uniform mat4 worldView;\n"
//            "void main()\n"
//            "{\n"
//            "    gl_Position = worldView * modelView * vec4(pos, 1);\n"
//            "    color = vec4(vertexColor, 1);\n"
//            "}\n";
//
//        const char *fragment_source =
//            "#version 330\n"
//            "in vec4 color;\n"
//            "out vec4 fragColor;\n"
//            "void main()\n"
//            "{\n"
//            "    fragColor = color;\n"
//            "}\n";
//
//        setup_shader(shader, vertex_source, fragment_source, "colored");
    }

    void setup_textured_shader(Shader *vs, Shader *fs) {
//        const char *vertex_source =
//            "#version 330\n"
//            "layout(location = 0)in vec3 pos;\n"
//            "layout(location = 1)in vec2 vertexUV;\n"
//            "out vec2 uv;\n"
//            "uniform mat4 modelView;\n"
//            "uniform mat4 worldView;\n"
//            "void main()\n"
//            "{\n"
//            "    gl_Position = worldView * modelView * vec4(pos, 1);\n"
//            "    uv = vertexUV;\n"
//            "}\n";
//
//        const char *fragment_source =
//            "#version 330\n"
//            "uniform sampler2D tex;\n"
//            "in vec2 uv;\n"
//            "out vec4 fragColor;\n"
//            "void main()\n"
//            "{\n"
//            "    fragColor = texture(tex, uv);\n"
//            "}\n";
//
//        setup_shader(shader, vertex_source, fragment_source, "textured");
    }

    void draw_cube(Surface *surface, Material *material) {
        surface->clear(AttachmentType::Color | AttachmentType::Depth);
        surface->draw(vao.get(), ebo.get(), material, ElementDrawInfo{0, 6 * 6});
    }

    void setup() {
//        renderer->initialize(PipelineOptions(
//            PolygonCullingOptions(true),
//            DepthBufferOptions(true, true, DepthFunction::PassGreater, 0, 1))
//        );
        renderer->initialize();
        vbo = unique_ptr<VertexBuffer>(renderer->make_vbo());
        ebo = unique_ptr<IndexBuffer8>(renderer->make_ebo8());
        vao = unique_ptr<VertexArray>(renderer->make_vao());
        colored_material = unique_ptr<Material>(renderer->make_material());
        textured_material = unique_ptr<Material>(renderer->make_material());
        colored_shader_v = unique_ptr<Shader>(renderer->make_shader());
        colored_shader_f = unique_ptr<Shader>(renderer->make_shader());
        textured_shader_v = unique_ptr<Shader>(renderer->make_shader());
        textured_shader_f = unique_ptr<Shader>(renderer->make_shader());
        framebuffer = unique_ptr<FrameBufferSurface>(renderer->make_framebuffer());
        render_texture = shared_ptr<Texture>(renderer->make_texture(TextureInitOptions()));
        render_depth_buffer = unique_ptr<RenderBuffer>(renderer->make_renderbuffer());

        float vertices[8 * 8] = {
            // x,y,z, u,v, r,g,v
            0, 0, 1,  0, 0,  0, 0, 1,
            1, 0, 1,  1, 0,  1, 0, 1,
            0, 0, 0,  1, 0,  0, 0, 0,
            1, 0, 0,  0, 0,  1, 0, 0,

            0, 1, 1,  0, 1,  0, 1, 1,
            1, 1, 1,  1, 1,  1, 1, 1,
            0, 1, 0,  1, 1,  0, 1, 0,
            1, 1, 0,  0, 1,  1, 1, 0,
        };

        unsigned char elements[12 * 3] = {
            1, 0, 3, 3, 0, 2,  // bottom
            4, 5, 6, 6, 5, 7,  // top
            0, 1, 4, 4, 1, 5,  // front side
            3, 2, 7, 7, 2, 6,  // back side
            2, 0, 6, 6, 0, 4,  // left side
            1, 3, 5, 5, 3, 7,  // right side
        };

        vbo->set_data(0, vertices, 8 * 8 * sizeof(float));
        VertexAttributeLayout layout(3, 8 * sizeof(float));
        layout.add_attribute(3, sizeof(float), 0);
        layout.add_attribute(2, sizeof(float), 3 * sizeof(float));
        layout.add_attribute(3, sizeof(float), 5 * sizeof(float));
        vao->add_buffer(vbo.get(), layout);

        ebo->set(0, elements, 6 * 6);

        setup_colored_shader(colored_shader_v.get(), colored_shader_f.get());
        setup_textured_shader(textured_shader_v.get(), textured_shader_f.get());

        array<const Shader *, 2> shaders = {
            colored_shader_v.get(), colored_shader_f.get()
        };
        colored_material->link_shaders(shaders.data(), shaders.size());
        shaders = {
            textured_shader_v.get(), textured_shader_f.get()
        };
        textured_material->link_shaders(shaders.data(), shaders.size());
        std::weak_ptr<Texture> material_textures[1] = { render_texture };
        textured_material->set_textures(material_textures, 1);
        textured_material->set_uniform_texture("tex", 0);

        // Setup framebuffer
        render_texture->create_storage(512, 512, TextureFormat::RGB);
        render_depth_buffer->create_storage(512, 512, TextureFormat::Depth16);
        framebuffer->attach_texture(render_texture.get(), AttachmentType::Color);
        framebuffer->attach_renderbuffer(render_depth_buffer.get(), AttachmentType::Depth);
        framebuffer->set_viewport(0, 0, 512, 512);

        Matrix4x4 framebuffer_worldview = Matrix4x4::perspective(1, 1.0f, 10.0f, 60 * (3.14159f / 180.0f));
        Matrix4x4 world_view;
        Matrix4x4 model_view;
        colored_material->set_uniform_matrix4x4("worldView", framebuffer_worldview);

        renderer->main_surface()->set_clear_color(Vector4(0.5f, 0.5f, 1.0f, 1.0f));
    }

    void update(float elapsed_seconds) override {
        // Update shared model view rotation
        current_rotation += elapsed_seconds;
        Matrix4x4 model_view =
            Matrix4x4::translate(Vector4(0, 0, -2.0f, 1))
            * Matrix4x4::rotation_y(current_rotation)
            * Matrix4x4::translate(Vector4(-0.5f, -0.5f, -0.5f, 1));

        // Draw cube on framebuffer
        colored_material->set_uniform_matrix4x4("modelView", model_view);
        draw_cube(framebuffer.get(), colored_material.get());

        // Draw cube on main surface
        Surface *main_surface = renderer->main_surface();
        auto width = main_surface->width();
        auto height = main_surface->height();
        main_surface->set_viewport(0, 0, width, height);
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        Matrix4x4 world_view = Matrix4x4::perspective(aspect, 1.0f, 10.0f, 60.0f * (3.14159f / 180.0f));
        textured_material->set_uniform_matrix4x4("worldView", world_view);
        textured_material->set_uniform_matrix4x4("modelView", model_view);
        draw_cube(renderer->main_surface(), textured_material.get());
        renderer->present();
    }

};

int main(int argc, char **argv) {
    unique_ptr<GLFWContext> window(new GLFWContext(GLFWWindowInitOptions()));
    unique_ptr<Renderer> renderer(giygas::make_renderer(window.get()));
    FramebufferExample app(renderer.get());
    app.setup();
    EventLoopContextRunner runner(window.get(), &app);
    window->show();
    runner.run();
	return 0;
}
