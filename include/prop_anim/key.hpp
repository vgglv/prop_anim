#pragma once
#include <memory>
#include <any>

namespace prop_anim {
	struct AnimKey;
	using KeyUniqPtr = std::unique_ptr<AnimKey>;
	using AnyValue = std::any;

	struct AnimKey {
		explicit AnimKey();
		AnimKey(const AnimKey&) = delete;
		AnimKey& operator=(const AnimKey&) = delete;

		AnimKey(AnimKey&&) noexcept = default;
		AnimKey& operator=(AnimKey&&) noexcept = default;
		float time = 0.f;
		AnyValue value;
		uint32_t idx = 0;
	};
}
