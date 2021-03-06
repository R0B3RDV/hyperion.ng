#pragma once

// hyperion incluse
#include "ProviderSpi.h"


///
/// Implementation of the LedDevice interface for writing to APA102 led device.
///
class LedDeviceAPA102 : public ProviderSpi
{
public:
	///
	/// Constructs specific LedDevice
	///
	/// @param deviceConfig json device config
	///
	LedDeviceAPA102(const Json::Value &deviceConfig);

	/// constructs leddevice
	static LedDevice* construct(const Json::Value &deviceConfig);

private:
	///
	/// Writes the led color values to the led-device
	///
	/// @param ledValues The color-value per led
	/// @return Zero on succes else negative
	///
	virtual int write(const std::vector<ColorRgb> &ledValues);
};
