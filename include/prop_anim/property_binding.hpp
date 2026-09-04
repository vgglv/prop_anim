#pragma once
#include "key.hpp"
#include <string>
#include <utility>

namespace prop_anim {
	class PropertyBinding {
	public:
		explicit PropertyBinding(std::string  _type_name) : type_name(std::move(_type_name)) {}
		virtual ~PropertyBinding() = default;
		virtual void set(const AnyValue& value) = 0;
		std::string type_name;
	};
}