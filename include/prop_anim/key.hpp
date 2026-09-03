#pragma once
#include <memory>
#include <any>

namespace prop_anim {
	struct AnimKey;
	using KeyUniqPtr = std::unique_ptr<AnimKey>;
	using AnyValue = std::any;

	struct AnimKey {
		explicit AnimKey();
		float time = 0.f;
		AnyValue value;
		uint32_t idx = 0;
	};
}
