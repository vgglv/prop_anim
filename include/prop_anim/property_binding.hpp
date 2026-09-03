#pragma once
#include "key.hpp"

namespace prop_anim {
	class PropertyBinding {
	public:
		virtual ~PropertyBinding() = default;
		virtual void set(const AnyValue& value) = 0;
	};
}