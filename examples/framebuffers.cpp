#include <giygas/giygas.hpp>
#include <giygas/GLFWWindow.hpp>
#include <memory>
#include <iostream>

using namespace giygas;
using namespace std;

class FramebufferExample
{
public:
    shared_ptr<GLFWWindow> window;
    unique_ptr<Renderer> renderer;
    unique_ptr<VertexBuffer> vbo;
    unique_ptr<ElementBufferChar> ebo;
    unique_ptr<VertexArray> vao;
    unique_ptr<Material> colored_material;
    unique_ptr<Material> textured_material;
    shared_ptr<Shader> colored_shader;
    shared_ptr<Shader> textured_shader;
    unique_ptr<FrameBufferSurface> framebuffer;
    shared_ptr<Texture> render_texture;
    unique_ptr<RenderBuffer> render_depth_buffer;

    FramebufferExample()
        : window(new GLFWWindow)
    {
    }

    void setup_shader(
            Shader &shader,
            const char *vertex_source,
            const char *fragment_source,
            const char *shader_name
    ) {
        shader.set_from_source(vertex_source, fragment_source);
        if (!shader.is_valid()) {
            cout << "Shader \"" << shader_name << "\" is invalid." << endl
                 << "Vertex message: " << shader.get_vertex_message() << endl
                 << "Fragment message: " << shader.get_fragment_message() << endl;
        }
    }

    void setup_colored_shader(Shader &shader) {
        const char *vertex_source =
            "#version 330\n"
            "layout(location = 0)in vec3 pos;\n"
            "layout(location = 2)in vec3 vertexColor;\n"
            "out vec4 color;\n"
            "uniform mat4 modelView;\n"
            "uniform mat4 worldView;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(pos, 1) * modelView;\n"
            "    color = vec4(vertexColor, 1);\n"
            "}\n";

        const char *fragment_source =
            "#version 330\n"
            "//uniform sampler2D tex;\n"
            "in vec4 color;\n"
            "//in vec2 uv;\n"
            "out vec4 fragColor;\n"
            "void main()\n"
            "{\n"
            "    fragColor = color;\n"
            "}\n";

        setup_shader(shader, vertex_source, fragment_source, "colored");
    }

    void setup_textured_shader(Shader &shader) {
        const char *vertex_source =
            "#version 330\n"
            "layout(location = 0)in vec3 pos;\n"
            "layout(location = 1)in vec2 vertexUV;\n"
            "out vec2 uv;\n"
            "uniform mat4 modelView;\n"
            "uniform mat4 worldView;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(pos, 1) * modelView;\n"
            "    uv = vertexUV;\n"
            "}\n";

        const char *fragment_source =
            "#version 330\n"
            "uniform sampler2D tex;\n"
            "//in vec4 color;\n"
            "in vec2 uv;\n"
            "out vec4 fragColor;\n"
            "void main()\n"
            "{\n"
            "    fragColor = texture(tex, uv);\n"
            "    //fragColor = vec4(1, 0, 1, 1);\n"
            "}\n";

        setup_shader(shader, vertex_source, fragment_source, "textured");
    }

    void draw_cube(Surface *surface, Material *material) {
        surface->clear(SurfaceBufferType::Color | SurfaceBufferType::Depth);
        surface->draw(vao.get(), ebo.get(), material, ElementDrawInfo{0, 3 * 6});
    }

    void run() {
        window->set_show_hints(3, 2);
        window->show();

        renderer = unique_ptr<Renderer>(giygas::make_renderer(window));
        vbo = unique_ptr<VertexBuffer>(renderer->make_vbo());
        ebo = unique_ptr<ElementBufferChar>(renderer->make_char_ebo());
        vao = unique_ptr<VertexArray>(renderer->make_vao());
        colored_material = unique_ptr<Material>(renderer->make_material());
        textured_material = unique_ptr<Material>(renderer->make_material());
        colored_shader = shared_ptr<Shader>(renderer->make_shader());
        textured_shader = shared_ptr<Shader>(renderer->make_shader());
        framebuffer = unique_ptr<FrameBufferSurface>(renderer->make_framebuffer());
        render_texture = shared_ptr<Texture>(renderer->make_texture());
        render_depth_buffer = unique_ptr<RenderBuffer>(renderer->make_renderbuffer());

        float vertices[8 * 8] = {
            // x,y,z, u,v, r,g,v
            0, 0, 0,  0, 0,  0, 0, 0,
            1, 0, 0,  1, 0,  1, 0, 0,
            0, 0, 1,  0, 0,  0, 0, 1,
            1, 0, 1,  1, 0,  1, 0, 1,

            0, 1, 0,  0, 1,  0, 1, 0,
            1, 1, 0,  1, 1,  1, 1, 0,
            0, 1, 1,  0, 1,  0, 1, 1,
            1, 1, 1,  1, 1,  1, 1, 1,
        };

        unsigned char elements[12 * 3] = {
            0, 1, 2, 2, 1, 3,  // bottom
            4, 5, 6, 6, 5, 7,  // top
            0, 1, 4, 4, 1, 5, //side
        };

        vbo->set_data(0, vertices, 8 * 8 * sizeof(float));
        VertexAttributeLayout layout(3, 8 * sizeof(float));
        layout.add_attribute(3, sizeof(float), 0);
        layout.add_attribute(2, sizeof(float), 3 * sizeof(float));
        layout.add_attribute(3, sizeof(float), 5 * sizeof(float));
        vao->add_buffer(vbo.get(), layout);

        ebo->set(0, elements, 6 * 3);

        setup_colored_shader(*colored_shader.get());
        setup_textured_shader(*textured_shader.get());

        colored_material->set_shader(colored_shader);
        textured_material->set_shader(textured_shader);
        std::weak_ptr<Texture> material_textures[1] = { render_texture };
        textured_material->set_textures(material_textures, 1);
        textured_material->set_uniform_texture("tex", 0);

        // Setup framebuffer
        render_texture->create_storage(512, 512, TextureFormat::RGB);
        render_depth_buffer->create_storage(512, 512, TextureFormat::Depth16);
        framebuffer->attach_texture(render_texture.get(), SurfaceBufferType::Color);
        framebuffer->attach_renderbuffer(render_depth_buffer.get(), SurfaceBufferType::Depth);
        framebuffer->set_viewport(0, 0, 512, 512);
        Matrix4x4 world_view = Matrix4x4::perspective(1, 0, 10, 60.0f * 3.14159f / 180.0f);
        Matrix4x4 model_view = Matrix4x4::translate(Vector4(0, 0, 0, 1));

        float current_rotation = 0.0f;

        bool is_running = true;
        while (is_running) {
            colored_material->set_uniform_matrix4x4("modelView", model_view);
            textured_material->set_uniform_matrix4x4("modelView", model_view);

            Surface *main_surface = renderer->main_surface();
            main_surface->set_viewport(0, 0, main_surface->width(), main_surface->height());

            draw_cube(framebuffer.get(), colored_material.get());
            draw_cube(renderer->main_surface(), textured_material.get());

            window->update();
            is_running = !window->should_close();
        }
    }

};

int main(int argc, char **argv)
{
    FramebufferExample app;
    app.run();
	return 0;
}
