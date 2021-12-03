#include "VertexBuffer.h"

#include "../Graphics/Graphics.h"

inline VertexBuffer::VertexBuffer(Graphics& graphics, std::vector<Vertex>& verticies)
	: stride(sizeof(Vertex)), graphics(graphics)
{

}

inline void VertexBuffer::bind()
{
	get_context(graphics)->IASetVertexBuffers(0u, 1u, vbuffer_com.GetAddressOf(), &stride, &offset);
}