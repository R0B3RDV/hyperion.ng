
// STL includes
#include <cerrno>
#include <cstring>

// Local LedDevice includes
#include "LedDeviceTinkerforge.h"

static const unsigned MAX_NUM_LEDS = 320;
static const unsigned MAX_NUM_LEDS_SETTABLE = 16;

LedDeviceTinkerforge::LedDeviceTinkerforge(const Json::Value &deviceConfig)
	: LedDevice()
	, _ipConnection(nullptr)
	, _ledStrip(nullptr)
	, _colorChannelSize(0)
{
	setConfig(deviceConfig);
}

LedDeviceTinkerforge::~LedDeviceTinkerforge()
{
	// Close the device (if it is opened)
	if (_ipConnection != nullptr && _ledStrip != nullptr)
	{
		switchOff();
	}

	// Clean up claimed resources
	delete _ipConnection;
	delete _ledStrip;
}

bool LedDeviceTinkerforge::setConfig(const Json::Value &deviceConfig)
{
	_host     = deviceConfig.get("output", "127.0.0.1").asString();
	_port     = deviceConfig.get("port", 4223).asInt();
	_uid      = deviceConfig["uid"].asString();
	_interval = deviceConfig["rate"].asInt();

	return true;
}

LedDevice* LedDeviceTinkerforge::construct(const Json::Value &deviceConfig)
{
	return new LedDeviceTinkerforge(deviceConfig);
}

int LedDeviceTinkerforge::open()
{
	// Check if connection is already createds
	if (_ipConnection != nullptr)
	{
		Error(_log, "Attempt to open existing connection; close before opening");
		return -1;
	}

	// Initialise a new connection
	_ipConnection = new IPConnection;
	ipcon_create(_ipConnection);

	int connectionStatus = ipcon_connect(_ipConnection, _host.c_str(), _port);
	if (connectionStatus < 0) 
	{
		Warning(_log, "Attempt to connect to master brick (%s:%d) failed with status %d", _host.c_str(), _port, connectionStatus);
		return -1;
	}

	// Create the 'LedStrip'
	_ledStrip = new LEDStrip;
	led_strip_create(_ledStrip, _uid.c_str(), _ipConnection);

	int frameStatus = led_strip_set_frame_duration(_ledStrip, _interval);
	if (frameStatus < 0) 
	{
		Error(_log,"Attempt to connect to led strip bricklet (led_strip_set_frame_duration()) failed with status %d", frameStatus);
		return -1;
	}

	return 0;
}

int LedDeviceTinkerforge::write(const std::vector<ColorRgb> &ledValues)
{
	if ((unsigned)_ledCount > MAX_NUM_LEDS) 
	{
		Error(_log,"Invalid attempt to write led values. Not more than %d leds are allowed.", MAX_NUM_LEDS);
		return -1;
	}

	if (_colorChannelSize < (unsigned)_ledCount)
	{
		_redChannel.resize(_ledCount, uint8_t(0));
		_greenChannel.resize(_ledCount, uint8_t(0));
		_blueChannel.resize(_ledCount, uint8_t(0));
	}
	_colorChannelSize = _ledCount;

	auto redIt   = _redChannel.begin();
	auto greenIt = _greenChannel.begin();
	auto blueIt  = _blueChannel.begin();

	for (const ColorRgb &ledValue : ledValues)
	{
		*redIt = ledValue.red;
		++redIt;
		*greenIt = ledValue.green;
		++greenIt;
		*blueIt = ledValue.blue;
		++blueIt;
	}

	return transferLedData(_ledStrip, 0, _colorChannelSize, _redChannel.data(), _greenChannel.data(), _blueChannel.data());
}

int LedDeviceTinkerforge::transferLedData(LEDStrip *ledStrip, unsigned index, unsigned length, uint8_t *redChannel, uint8_t *greenChannel, uint8_t *blueChannel) 
{
	if (length == 0 || index >= length || length > MAX_NUM_LEDS)
	{
		return E_INVALID_PARAMETER;
	}

	uint8_t reds[MAX_NUM_LEDS_SETTABLE];
	uint8_t greens[MAX_NUM_LEDS_SETTABLE];
	uint8_t blues[MAX_NUM_LEDS_SETTABLE];

	for (unsigned i=index; i<length; i+=MAX_NUM_LEDS_SETTABLE)
	{
		const unsigned copyLength = (i + MAX_NUM_LEDS_SETTABLE > length) ? length - i : MAX_NUM_LEDS_SETTABLE;
		memcpy(reds,   redChannel   + i,   copyLength);
		memcpy(greens, greenChannel + i, copyLength);
		memcpy(blues,  blueChannel  + i,  copyLength);

		const int status = led_strip_set_rgb_values(ledStrip, i, copyLength, reds, greens, blues);
		if (status != E_OK)
		{
			Warning(_log, "Setting led values failed with status %d", status);
			return status;
		}
	}

	return E_OK;
}
