#include "prop_anim/easing.hpp"

namespace prop_anim {
	float apply_easing(const EasingType& e, float t) {
		switch (e) {
			case EasingType::Linear:
				return t;
			case EasingType::EaseIn:
				return t * t;
			case EasingType::EaseOut:
				return 1.0f - ((1.0f - t) * (1.0f - t));
			case EasingType::EaseInOut:
				if (t < 0.5f) {
					return 2.0f * t * t;
				} else {
					float a = (-2.0f * t) + 2.0f;
					return 1.0f - (a * a * 0.5f);
				}
		}
		return t;
	}
}