#ifndef __SEATALK__MESSAGE_54__HPP__
#define __SEATALK__MESSAGE_54__HPP__

#include <marnav/seatalk/message.hpp>

namespace marnav
{
namespace seatalk
{

/// @brief GMT-Time
///
/// @code
/// 54 T1 RS HH
///
/// GMT-time:
///     HH hours,
///     6 MSBits of RST = minutes = (RS & 0xFC) / 4
///     6 LSBits of RST = seconds =  ST & 0x3F
/// @endcode
///
/// Corresponding NMEA sentences: RMC, GAA, BWR, BWC
///
class message_54 : public message
{
public:
	constexpr static const message_id ID = message_id::gmt_time;

	message_54();
	message_54(const message_54 &) = default;
	message_54 & operator=(const message_54 &) = default;

	virtual raw get_data() const override;

	static std::unique_ptr<message> parse(const raw & data);

private:
	uint8_t hour;
	uint8_t minute;
	uint8_t second;

public:
	uint8_t get_hour() const noexcept { return hour; }
	uint8_t get_minute() const noexcept { return minute; }
	uint8_t get_second() const noexcept { return second; }

	void set_time(uint8_t h, uint8_t m, uint8_t s) noexcept;
};
}
}

#endif