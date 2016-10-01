
#ifndef __WINTK_SHARED_CX_H
#define __WINTK_SHARED_CX_H

/** @name WCPToolkit
 *  windows toolkit utility functions
 */
///@{
namespace WCPToolkit
{

#if defined(OS_W8)
	namespace Sensors
	{
		namespace Geolocator
		{
			public ref class Location sealed
			{
			public:
				property bool  HasValidCoordonates;
				property float Longitude;
				property float Latitude;
				property float Altitude;
				property float Accuracy;

				static Location^ Get();
			};
		}
	}


#endif
}
#endif // __WINTK_SHARED_CX_H
