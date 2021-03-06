// hyperion local includes
#include "LedDeviceAtmo.h"

LedDeviceAtmo::LedDeviceAtmo(const Json::Value &deviceConfig)
	: ProviderRs232(deviceConfig)
{
	_ledBuffer.resize(4 + 5*3); // 4-byte header, 5 RGB values
	_ledBuffer[0] = 0xFF; // Startbyte
	_ledBuffer[1] = 0x00; // StartChannel(Low)
	_ledBuffer[2] = 0x00; // StartChannel(High)
	_ledBuffer[3] = 0x0F; // Number of Databytes send (always! 15)
}

LedDevice* LedDeviceAtmo::construct(const Json::Value &deviceConfig)
{
	return new LedDeviceAtmo(deviceConfig);
}


int LedDeviceAtmo::write(const std::vector<ColorRgb> &ledValues)
{
	// The protocol is shomehow limited. we always need to send exactly 5 channels + header
	// (19 bytes) for the hardware to recognize the data
	if (_ledCount != 5)
	{
		Error( _log, "%d channels configured. This should always be 5!", _ledCount);
		return 0;
	}

	// write data
	memcpy(4 + _ledBuffer.data(), ledValues.data(), _ledCount * sizeof(ColorRgb));
	return writeBytes(_ledBuffer.size(), _ledBuffer.data());
}

