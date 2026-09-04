#include <prop_anim/anim_vector.hpp>
#include <algorithm>
#include <cassert>

namespace prop_anim {
	std::unique_ptr<AnimVector> AnimVector::create() {
		return std::make_unique<AnimVector>();
	}

	const std::vector<AnimUniqPtr>& AnimVector::get_animations() const {
		return _animations_vec;
	}

	void AnimVector::add_animation(AnimUniqPtr anim) {
		auto it = std::find_if(_animations_vec.begin(), _animations_vec.end(), [name = anim->name()](const AnimUniqPtr& anim) {
			return anim->name() == name;
		});
		if (it != _animations_vec.end()) {
			static_assert(true, "already adding existing animation!");
			return;
		}
		_animations_vec.emplace_back(std::move(anim));
	}

	void AnimVector::remove_animation(const std::string& name) {
		const auto& it = std::find_if(_animations_vec.begin(), _animations_vec.end(), [name](const AnimUniqPtr& animation) {
			return name == animation->name();
		});
		if (it != _animations_vec.end()) {
			_animations_vec.erase(it);
		}
	}

	[[nodiscard]] Anim* AnimVector::find_animation(const std::string& name) const {
		const auto& it = std::find_if(_animations_vec.begin(), _animations_vec.end(), [name](const AnimUniqPtr& animation) {
			return name == animation->name();
		});
		if (it != _animations_vec.end()) {
			return (*it).get();
		}
		return nullptr;
	}

	[[nodiscard]] Anim* AnimVector::current_animation() const {
		return _current_animation;
	}

	[[nodiscard]] size_t AnimVector::size() const {
		return _animations_vec.size();
	}

	void AnimVector::clear() {
		_animations_vec.clear();
	}

	void AnimVector::update(float t) {
		if (_current_animation) {
			_current_animation->update(t);
		}
	}

	void AnimVector::set_current_animation(const std::string& name) {
		if (_current_animation) {
			for (const auto& track : _current_animation->get_tracks()) {
				track->setup_pose();
			}
		}
		_current_animation = find_animation(name);
	}

	void AnimVector::clear_current_animation() {
		_current_animation = nullptr;
	}
}