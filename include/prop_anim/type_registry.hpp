#pragma once
#include <functional>
#include <typeindex>
#include <unordered_map>
#include "key.hpp"

namespace prop_anim {
	class TypeRegistry {
	public:
		template<typename T>
		void register_type(const std::string& name, std::function<T(const T&, const T&, float)> fn) {
			_interpolators[name] = [fn = std::move(fn)](const AnyValue& a, const AnyValue& b, float t) -> AnyValue {
				return fn(std::any_cast<const T&>(a), std::any_cast<const T&>(b), t);
			};
		}

		[[nodiscard]] AnyValue interpolate(const std::string& name, const AnyValue& a, const AnyValue& b, float t) const {
			auto it = _interpolators.find(name);

			if (it == _interpolators.end())
				throw std::runtime_error("No interpolator registered");

			return it->second(a, b, t);
		}

	private:
		std::unordered_map<std::string, std::function<AnyValue(const AnyValue&, const AnyValue&, float)>> _interpolators;
	};
}