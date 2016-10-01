#pragma once

namespace WCPToolkit
{
	namespace Utils
	{
		namespace ZOrdering
		{
			public enum class ZPriority
			{
				BANNER = 0,
				INTERSTITIAL_NORMAL = 50,
				INTERSTITIAL_HIGH = 100
			};

			[Windows::Foundation::Metadata::WebHostHidden]
			public ref class ZOrder sealed
			{
			public:
				static void SetZPriority(Windows::UI::Xaml::UIElement^ elem, ZPriority priority);

			};
		}
	}
}