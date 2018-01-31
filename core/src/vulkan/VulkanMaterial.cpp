#include "VulkanMaterial.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanMaterial::VulkanMaterial(VulkanRenderer *renderer) {
    _renderer = renderer;
}

VulkanMaterial::VulkanMaterial(VulkanMaterial &&) {

}

VulkanMaterial& VulkanMaterial::operator=(VulkanMaterial &&) {

    return *this;
}

void VulkanMaterial::move_common(VulkanMaterial &&other) {
    _renderer = other._renderer;


}

VulkanMaterial::~VulkanMaterial() {


}