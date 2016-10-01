namespace WCPToolkit
{
    namespace Utils
    {
        namespace ZOrdering
        {
            public enum ZPriority
            {
                BANNER = 0,
                INTERSTITIAL_NORMAL = 50,
                INTERSTITIAL_HIGH = 100
            }

            public sealed class ZOrder
            {
#if OS_W8
                public static void SetZPriority(Windows.UI.Xaml.UIElement elem, ZPriority priority)
                {
                    Windows.UI.Xaml.Controls.Canvas.SetZIndex(elem, (int)priority);
                }
#else
                public static void SetZPriority(System.Windows.UIElement elem, ZPriority priority)
                {
                    System.Windows.Controls.Canvas.SetZIndex(elem, (int)priority);
                }
#endif

            }
        }
    }
}
