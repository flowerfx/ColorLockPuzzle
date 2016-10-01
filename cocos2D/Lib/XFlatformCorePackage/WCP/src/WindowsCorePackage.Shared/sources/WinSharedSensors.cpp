#include "pch.h"

#include "WinShared.h"
#include "WinSharedCX.h"


#if defined(OS_WP81)
#include "client.h"
#endif

#ifdef OS_W10
#include "wrl/client.h"
#endif

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
#if (WCP_TEST_SAMPLE == 1)
	float accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z;

	void WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEAccelerometerStart(unsigned int interval)
	{
		WCPToolkit::Sensors::Accelerometer::Start(interval);
	}
	void WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEAccelerometerStop()
	{
		WCPToolkit::Sensors::Accelerometer::Stop();
	}
	void WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEAccelerometerUpdate()
	{
		WCPToolkit::Sensors::Accelerometer::GetReading(accel_x, accel_y, accel_z);
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEAccelerometerX()
	{
		return accel_x;
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEAccelerometerY()
	{
		return accel_y;
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEAccelerometerZ()
	{
		return accel_z;
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGyrometerStart(unsigned int interval)
	{
		WCPToolkit::Sensors::Gyrometer::Start(interval);
	}
	void WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGyrometerStop()
	{
		WCPToolkit::Sensors::Gyrometer::Stop();
	}
	void WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGyrometerUpdate()
	{
		WCPToolkit::Sensors::Gyrometer::GetReading(gyro_x, gyro_y, gyro_z);
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGyrometerX()
	{
		return gyro_x;
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGyrometerY()
	{
		return gyro_y;
	}
	float WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGyrometerZ()
	{
		return gyro_z;
	}
#endif
}
#endif





namespace WCPToolkit
{

#if defined(OS_W8)

	Sensors::Geolocator::Location^ Sensors::Geolocator::Location::Get()
	{
		float Latitude = 0;
		float Longitude = 0;
		float Altitude = 0;
		float Accuracy = 0;

		bool isValid = WCPToolkit::Sensors::Geolocator::GetLocation(Latitude, Longitude, Altitude, Accuracy);

		Sensors::Geolocator::Location^ pos = ref new Sensors::Geolocator::Location();
		pos->Latitude = Latitude;
		pos->Longitude = Longitude;
		pos->Altitude = Altitude;
		pos->Accuracy = Accuracy;
		pos->HasValidCoordonates = isValid;

		return pos;
	}

#endif

#pragma region Gyrometer

	static Windows::Devices::Sensors::Gyrometer^ g_Gyrometer = nullptr;

	void Sensors::Gyrometer::Start(unsigned int nFreq)
	{
		if (!g_Gyrometer)
		{
			try
			{
				g_Gyrometer = Windows::Devices::Sensors::Gyrometer::GetDefault();
			}
			catch (Platform::Exception^ e)
			{
			}

			if (!g_Gyrometer)
				return;
		}

		if (g_Gyrometer->ReportInterval == 0)
			g_Gyrometer->ReportInterval = nFreq;
	}

	void Sensors::Gyrometer::Stop()
	{
		if (g_Gyrometer)
		{
			g_Gyrometer->ReportInterval = 0;
			g_Gyrometer = nullptr;
		}
	}

	bool Sensors::Gyrometer::GetReading(float& x, float& y, float& z)
	{
		//ID_CAP_SENSORS in WMAppManifest.xml
		// call smth like GetAppInputMgr().GetGamepad(0).RaiseGyroscopeEvent(x, y, z) from glf

		if (!g_Gyrometer)
			return false;
		if (g_Gyrometer->ReportInterval == 0)
			return false;

		Windows::Devices::Sensors::GyrometerReading^ gr = g_Gyrometer->GetCurrentReading();
		if (!gr)
			return false;

		x = (float)gr->AngularVelocityX;
		y = (float)gr->AngularVelocityY;
		z = (float)gr->AngularVelocityZ;

		return true;
	}
#pragma endregion Gyrometer


#pragma region Accelerometer

	static Windows::Devices::Sensors::Accelerometer^ g_Accelerometer = nullptr;

	void Sensors::Accelerometer::Start(unsigned int nFreq)
	{
		if (!g_Accelerometer)
		{
			try
			{
				g_Accelerometer = Windows::Devices::Sensors::Accelerometer::GetDefault();
			}
			catch (Platform::Exception^ e)
			{
			}

			if (!g_Accelerometer)
				return;
		}

		g_Accelerometer->ReportInterval = nFreq;
	}

	void Sensors::Accelerometer::Stop()
	{
		if (g_Accelerometer)
		{
			g_Accelerometer->ReportInterval = 0;
			g_Accelerometer = nullptr;
		}
	}

	bool Sensors::Accelerometer::GetReading(float& x, float& y, float& z)
	{
		//ID_CAP_SENSORS in WMAppManifest.xml

		if (!g_Accelerometer)
			return false;
		if (g_Accelerometer->ReportInterval == 0)
			return false;

		Windows::Devices::Sensors::AccelerometerReading^ gr = g_Accelerometer->GetCurrentReading();

		if (!gr)
			return false;

		x = (float)gr->AccelerationX;
		y = (float)gr->AccelerationY;
		z = (float)gr->AccelerationZ;

		return true;
	}
#pragma endregion Accelerometer

#pragma region Geolocator
#if defined(OS_W8)

	static Windows::Devices::Geolocation::Geolocator^ g_Geolocator = nullptr;
	static Windows::Foundation::EventRegistrationToken g_PositionChangedRegistrationToken;
	static Windows::Foundation::EventRegistrationToken g_StatusChangedRegistrationToken;

	static float g_dLatitude = 0;
	static float g_dLongitude = 0;
	static float g_dAltitude = 0;
	static float g_dAccuracy = 0;
	static bool g_bHasLastKnown = false;
	static Sensors::Geolocator::GeolocationStatus g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::Disabled;
	static Sensors::Geolocator::GeolocationSource g_positionSource = Sensors::Geolocator::GeolocationSource::Unknown;

	void Sensors::Geolocator::Start(unsigned int ReportInterval)
	{
		if (g_Geolocator != nullptr)
		{
			g_Geolocator->ReportInterval = ReportInterval;
			return;
		}

		WCPToolkit::Utils::RunOnUIThread([ReportInterval]()
		{
			g_Geolocator = ref new  Windows::Devices::Geolocation::Geolocator();
			g_Geolocator->DesiredAccuracy = Windows::Devices::Geolocation::PositionAccuracy::Default;
			g_Geolocator->ReportInterval = ReportInterval;

			g_PositionChangedRegistrationToken = g_Geolocator->PositionChanged += ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Geolocation::Geolocator ^, Windows::Devices::Geolocation::PositionChangedEventArgs ^>([](Windows::Devices::Geolocation::Geolocator ^sender, Windows::Devices::Geolocation::PositionChangedEventArgs ^args){

				g_bHasLastKnown = true;

				g_dLatitude = (float)args->Position->Coordinate->Point->Position.Latitude;
				g_dLongitude = (float)args->Position->Coordinate->Point->Position.Longitude;
				g_dAltitude = (float)args->Position->Coordinate->Point->Position.Altitude;
				g_dAccuracy = (float)args->Position->Coordinate->Accuracy;

				switch (args->Position->Coordinate->PositionSource)
				{
				case Windows::Devices::Geolocation::PositionSource::Cellular:
					g_positionSource = Sensors::Geolocator::GeolocationSource::Cellular;
					break;
				case Windows::Devices::Geolocation::PositionSource::Satellite:
					g_positionSource = Sensors::Geolocator::GeolocationSource::Satellite;
					break;
				case Windows::Devices::Geolocation::PositionSource::WiFi:
					g_positionSource = Sensors::Geolocator::GeolocationSource::WiFi;
					break;
				case Windows::Devices::Geolocation::PositionSource::IPAddress:
					g_positionSource = Sensors::Geolocator::GeolocationSource::IPAddress;
					break;
				case Windows::Devices::Geolocation::PositionSource::Unknown:
					g_positionSource = Sensors::Geolocator::GeolocationSource::Unknown;
					break;
				}
			});

			g_StatusChangedRegistrationToken = g_Geolocator->StatusChanged += ref new Windows::Foundation::TypedEventHandler<Windows::Devices::Geolocation::Geolocator ^, Windows::Devices::Geolocation::StatusChangedEventArgs ^>([](Windows::Devices::Geolocation::Geolocator ^sender, Windows::Devices::Geolocation::StatusChangedEventArgs ^args){
				switch (args->Status)
				{
				case Windows::Devices::Geolocation::PositionStatus::Disabled:
					g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::Disabled;
					break;
				case Windows::Devices::Geolocation::PositionStatus::Initializing:
					g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::Initializing;
					break;
				case Windows::Devices::Geolocation::PositionStatus::NoData:
					g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::NoData;
					break;
				case Windows::Devices::Geolocation::PositionStatus::NotAvailable:
					g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::NotAvailable;
					break;
				case Windows::Devices::Geolocation::PositionStatus::NotInitialized:
					g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::NotInitialized;
					break;
				case Windows::Devices::Geolocation::PositionStatus::Ready:
					g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::Ready;
					break;
				}
			});
		});
	}

	void Sensors::Geolocator::Stop()
	{
		if (g_Geolocator == nullptr)
			return;

		g_Geolocator->PositionChanged -= g_PositionChangedRegistrationToken;
		g_Geolocator->StatusChanged -= g_StatusChangedRegistrationToken;

		g_Geolocator = nullptr;
		g_geolocationStatus = Sensors::Geolocator::GeolocationStatus::Disabled;

		g_dLatitude = 0;
		g_dLongitude = 0;
		g_dAltitude = 0;
		g_dAccuracy = 0;
		g_bHasLastKnown = false;

		g_positionSource = Sensors::Geolocator::GeolocationSource::Unknown;
	}

	Sensors::Geolocator::GeolocationStatus Sensors::Geolocator::GetStatus()
	{
		return g_geolocationStatus;
	}

	Sensors::Geolocator::GeolocationSource Sensors::Geolocator::GetSource()
	{
		return g_positionSource;
	}

	bool Sensors::Geolocator::HasValidValues()
	{
		return g_bHasLastKnown;
	}

	bool Sensors::Geolocator::GetLocation(float& latitude, float& longitude, float& altitude, float& accuracy)
	{
		if (!g_bHasLastKnown)
		{
			return false;
		}

		latitude = g_dLatitude;
		longitude = g_dLongitude;
		altitude = g_dAltitude;
		accuracy = g_dAccuracy;

		return true;
	}
#endif

#pragma endregion Geolocation

};
