#pragma once
#include "key.hpp"
#include "point.hpp"

namespace prop_anim {
	class BezierKey : public BaseKey {
	public:
		~BezierKey() override = default;

		Point in_handle{};
		Point out_handle{};
	};
}