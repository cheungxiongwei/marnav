#include <marnav/ais/message_19.hpp>
#include <marnav/ais/angle.hpp>

namespace marnav
{
namespace ais
{
constexpr message_id message_19::ID;
constexpr std::size_t message_19::SIZE_BITS;

message_19::message_19()
	: message_19(ID)
{
}

message_19::message_19(message_id id)
	: message(id)
	, shipname("@@@@@@@@@@@@@@@@@@@@")
{
}

message_19::message_19(const raw & bits)
	: message_19(ID)
{
	if (bits.size() != SIZE_BITS)
		throw std::invalid_argument{"invalid number of bits in message_19"};
	read_data(bits);
}

void message_19::read_data(const raw & bits)
{
	get(bits, repeat_indicator);
	get(bits, mmsi);
	get(bits, sog);
	get(bits, position_accuracy);
	get(bits, longitude_minutes);
	get(bits, latitude_minutes);
	get(bits, cog);
	get(bits, hdg);
	get(bits, timestamp);
	get(bits, shipname);
	get(bits, shiptype);
	get(bits, to_bow);
	get(bits, to_stern);
	get(bits, to_port);
	get(bits, to_starboard);
	get(bits, epfd_fix);
	get(bits, raim);
	get(bits, dte);
	get(bits, assigned);
}

raw message_19::get_data() const
{
	raw bits(SIZE_BITS);

	bits.set(type(), 0, 6);
	set(bits, repeat_indicator);
	set(bits, mmsi);
	set(bits, sog);
	set(bits, position_accuracy);
	set(bits, longitude_minutes);
	set(bits, latitude_minutes);
	set(bits, cog);
	set(bits, hdg);
	set(bits, timestamp);
	set(bits, shipname);
	set(bits, shiptype);
	set(bits, to_bow);
	set(bits, to_stern);
	set(bits, to_port);
	set(bits, to_starboard);
	set(bits, epfd_fix);
	set(bits, raim);
	set(bits, dte);
	set(bits, assigned);

	return bits;
}

std::optional<geo::longitude> message_19::get_lon() const
{
	if (longitude_minutes == longitude_not_available)
		return std::make_optional<geo::longitude>();
	return to_geo_longitude(longitude_minutes, longitude_minutes.count, angle_scale::I4);
}

std::optional<geo::latitude> message_19::get_lat() const
{
	if (latitude_minutes == latitude_not_available)
		return std::make_optional<geo::latitude>();
	return to_geo_latitude(latitude_minutes, latitude_minutes.count, angle_scale::I4);
}

void message_19::set_lon_unavailable()
{
	longitude_minutes = longitude_not_available;
}

void message_19::set_lat_unavailable()
{
	latitude_minutes = latitude_not_available;
}

void message_19::set_lon(const geo::longitude & t)
{
	longitude_minutes = to_longitude_minutes(t, longitude_minutes.count, angle_scale::I4);
}

void message_19::set_lat(const geo::latitude & t)
{
	latitude_minutes = to_latitude_minutes(t, latitude_minutes.count, angle_scale::I4);
}

void message_19::set_shipname(const std::string & t)
{
	if (t.size() > 20) {
		shipname = t.substr(0, 20);
	} else {
		shipname = t;
	}
}

std::optional<units::knots> message_19::get_sog() const noexcept
{
	// ignores special value of 1022 = 102.2 knots or faster

	if (sog == sog_not_available)
		return {};
	return units::knots{0.1 * sog};
}

void message_19::set_sog_unavailable()
{
	sog = sog_not_available;
}

void message_19::set_sog(units::velocity t)
{
	if (t.value() < 0.0)
		throw std::invalid_argument{"SOG less than zero"};

	const auto v = t.get<units::knots>();
	sog = std::min(sog_max, static_cast<uint32_t>(round(v * 10).value()));
}

vessel_dimension message_19::get_vessel_dimension() const noexcept
{
	return vessel_dimension(to_bow, to_stern, to_port, to_starboard);
}

void message_19::set_vessel_dimension(const vessel_dimension & t)
{
	to_bow = t.get_raw_to_bow();
	to_stern = t.get_raw_to_stern();
	to_port = t.get_raw_to_port();
	to_starboard = t.get_raw_to_starboard();
}
}
}
