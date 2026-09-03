#pragma once
#include "anim.hpp"

namespace prop_anim {
	class AnimVector {
	public:
		const std::vector<AnimUniqPtr>& get_animations() const;
		void add_animation(AnimUniqPtr anim);
		void remove_animation(const std::string& name);

		[[nodiscard]] Anim* find_animation(const std::string& name) const;
		[[nodiscard]] Anim* current_animation() const;
		[[nodiscard]] size_t size() const;

		void clear();
		void update(float t);
		void set_current_animation(const std::string& name);
		void clear_current_animation();
	private:
		std::vector<AnimUniqPtr> _animations_vec;
		Anim* _current_animation = nullptr;
	};
}
