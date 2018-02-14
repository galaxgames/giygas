#include <giygas/CommandBuffer.hpp>
#include <cassert>
#include "CommandBufferImpl.hpp"

using namespace giygas;
using namespace std;

CommandBuffer::CommandBuffer() {
    _impl = nullptr;
}

CommandBuffer::CommandBuffer(CommandBufferImpl *impl) {
    _impl = impl;
}

CommandBuffer::CommandBuffer(CommandBuffer &&other) noexcept {
    *this = move(other);
}

CommandBuffer& CommandBuffer::operator=(CommandBuffer &&other) noexcept {
    swap(_impl, other._impl);
    return *this;
}

CommandBuffer::~CommandBuffer() {
    if (_impl != nullptr) {
        _impl->release();
    }
}

void CommandBuffer::record(const DrawInfo &info) {
    assert(_impl != nullptr);
    _impl->record(info);
}

const CommandBufferImpl* CommandBuffer::impl() const {
    return _impl;
}