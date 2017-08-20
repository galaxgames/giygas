#pragma once

class VertexBuffer
{
    

public:

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&);
    virtual ~VertexBuffer();

    void SetVector2Channel();
    void SetVector3Channel();
    void SetVector4Channel();
};


