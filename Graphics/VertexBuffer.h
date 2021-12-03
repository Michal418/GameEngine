#pragma once
#include "../Graphics/IBindable.h"
#include "../Logger.h"

class VertexBuffer : public IBindable
{
public:
	VertexBuffer(Graphics& graphics, std::vector<Vertex>& verticies);
	VertexBuffer& operator=(VertexBuffer&) = default;

public:
	virtual void bind() override;

private:
	Graphics& graphics;
	const UINT stride;
	const UINT offset = 0u;
};

