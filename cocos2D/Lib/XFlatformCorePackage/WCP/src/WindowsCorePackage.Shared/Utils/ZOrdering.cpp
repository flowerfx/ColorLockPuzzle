#include "pch.h"

#include "ZOrdering.h"

namespace WCPToolkit
{
	namespace Utils
	{
		namespace ZOrdering
		{
			void ZOrder::SetZPriority(Windows::UI::Xaml::UIElement^ elem, ZPriority priority)
			{
				Windows::UI::Xaml::Controls::Canvas::SetZIndex(elem, (int)priority);
			}
		}
	}
}
