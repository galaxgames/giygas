#include <giygas/giygas.hpp>
#include <giygas/GLFWWindow.hpp>
#include <memory>

using namespace giygas;
using namespace std;

int main(int argc, char **argv)
{
    shared_ptr<GLFWWindow> window(new GLFWWindow);
    Renderer *renderer = giygas::make_renderer(window);
	return 0;
}