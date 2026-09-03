#include "prop_anim/key.hpp"
#include <algorithm>
#include <cassert>
#include <prop_anim/track.hpp>
#include <ranges>

namespace {
	uint32_t last_index = 0;
}

namespace prop_anim {
	Track::Track() : _idx(++last_index) {
	}

	Track::~Track() = default;

	TrackIdx Track::idx() const {
		return _idx;
	}

	float Track::get_total_duration() const {
		if (!_keys_vec.empty()) {
			return _keys_vec.back()->time;
		}
		return 0.f;
	}

	void Track::update(float t) {
		for (const auto& key : _keys_vec) {
			//TODO: interpolate
		}
	}

	void Track::insert_sorted(KeyUniqPtr key) {
		auto it = std::ranges::upper_bound(_keys_vec, key->time, std::ranges::less{}, [](const KeyUniqPtr& key) {
			return key->time;
		});
		_keys_vec.insert(it, std::move(key));
	}

	void Track::sort() {
		std::ranges::sort(_keys_vec, [](const KeyUniqPtr& a, const KeyUniqPtr& b) {
			return a->time < b->time;
		});
	}

	void Track::remove_key(uint32_t idx) {
		const auto& it = std::ranges::find_if(_keys_vec, [idx](const KeyUniqPtr& track) {
			return track->idx == idx;
		});
		if (it != _keys_vec.end()) {
			_keys_vec.erase(it);
		}
	}

	const std::vector<KeyUniqPtr>& Track::get_keys() const {
		return _keys_vec;
	}
}
