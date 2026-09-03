#pragma once

namespace prop_anim {
	enum class EasingType {
		Linear,
		EaseIn,
		EaseOut,
		EaseInOut,
	};

	float apply_easing(const EasingType& e, float t);
}