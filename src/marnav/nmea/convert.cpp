#include "convert.hpp"
#include <stdexcept>
#include <cassert>

namespace marnav
{
namespace nmea
{
direction convert_hemisphere(const geo::latitude & p) noexcept
{
	switch (p.hem()) {
		case geo::latitude::hemisphere::north:
			return direction::north;
		case geo::latitude::hemisphere::south:
			return direction::south;
		default:
			assert(false);
	}
	return direction::none;
}

direction convert_hemisphere(const geo::longitude & p) noexcept
{
	switch (p.hem()) {
		case geo::longitude::hemisphere::east:
			return direction::east;
		case geo::longitude::hemisphere::west:
			return direction::west;
		default:
			assert(false);
	}
	return direction::none;
}

geo::latitude::hemisphere convert_hemisphere_lat(direction t)
{
	switch (t) {
		case direction::north:
			return geo::latitude::hemisphere::north;
		case direction::south:
			return geo::latitude::hemisphere::south;
		default:
			throw std::invalid_argument{"invalid hemisphere for latitude"};
	}
}

geo::longitude::hemisphere convert_hemisphere_lon(direction t)
{
	switch (t) {
		case direction::east:
			return geo::longitude::hemisphere::east;
		case direction::west:
			return geo::longitude::hemisphere::west;
		default:
			throw std::invalid_argument{"invalid hemisphere for longitude"};
	}
}

/// \cond DEV
namespace detail
{
static geo::latitude convert_latitude(const geo::latitude & v, direction d)
{
	return {v.get(), convert_hemisphere_lat(d)};
}

static geo::longitude convert_longitude(const geo::longitude & v, direction d)
{
	return {v.get(), convert_hemisphere_lon(d)};
}
}
/// \endcond

utils::optional<geo::latitude> correct_hemisphere(
	const utils::optional<geo::latitude> & v, const utils::optional<direction> & d)
{
	if (v && d)
		return detail::convert_latitude(v.value(), d.value());
	return v;
}

utils::optional<geo::longitude> correct_hemisphere(
	const utils::optional<geo::longitude> & v, const utils::optional<direction> & d)
{
	if (v && d)
		return detail::convert_longitude(v.value(), d.value());
	return v;
}
}
}