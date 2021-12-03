#pragma once
#include "../Graphics/IBindable.h"
#include <memory>

class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void draw() = 0;
	virtual void add_bindable(std::unique_ptr<IBindable>) = 0;
};