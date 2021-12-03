#include "../Graphics/IBindable.h"

ID3D11DeviceContext* IBindable::get_context(Graphics& graphics)
{
	return graphics.m_context_com.Get();
}

ID3D11Device* IBindable::get_device(Graphics& graphics)
{
	return graphics.m_device_com.Get();
}
