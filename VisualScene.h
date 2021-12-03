#pragma once
#include "../Wnd/Graphics/Graphics.h"
#include "../Wnd/Graphics/IDrawable.h"
#include "../Wnd/Graphics/VertexBuffer.h"
#include <memory>

class VisualScene : public IDrawable
{
public:
	VisualScene(Graphics& graphics);

public:
	virtual void draw() override;
	virtual void add_bindable(std::unique_ptr<IBindable>) override;

private:
	Graphics& m_graphics;
	std::vector<std::unique_ptr<IBindable>> binds{};
};