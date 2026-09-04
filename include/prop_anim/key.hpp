#pragma once
#include <memory>
#include <any>

namespace prop_anim {
	struct BaseKey;
	using KeyUniqPtr = std::unique_ptr<BaseKey>;
	using AnyValue = std::any;

	class BaseKey {
	public:
		explicit BaseKey();
		virtual ~BaseKey() = default;
		BaseKey(const BaseKey&) = delete;
		BaseKey& operator=(const BaseKey&) = delete;

		BaseKey(BaseKey&&) noexcept = default;
		BaseKey& operator=(BaseKey&&) noexcept = default;

		static KeyUniqPtr create(float t);

		float time = 0.f;
		AnyValue value;
		uint32_t idx = 0;
	};

	struct Point {
		float x;
		float y;
	};

	class BezierKey : public BaseKey {
	public:
		~BezierKey() override = default;

		static std::unique_ptr<BezierKey> create(float t);

		Point in_handle{};
		Point out_handle{};
	};
}
