#pragma once
#include "../Graphics/Graphics.h"

class IBindable
{
public:
	virtual void bind() = 0;
	virtual ~IBindable() = default;

protected:
	static ID3D11DeviceContext* get_context(Graphics& graphics);
	static ID3D11Device* get_device(Graphics& graphics);
};