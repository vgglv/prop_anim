#include <prop_anim/track.hpp>
#include <algorithm>
#include <cassert>

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

	void Track::insert_sorted(KeyUniqPtr key) {
		auto it = std::upper_bound(_keys_vec.begin(), _keys_vec.end(), key->time, [](float time, const KeyUniqPtr& key) {
			return time < key->time;
		});
		_keys_vec.insert(it, std::move(key));
	}

	void Track::sort() {
		std::sort(_keys_vec.begin(), _keys_vec.end(), [](const KeyUniqPtr& a, const KeyUniqPtr& b) {
			return a->time < b->time;
		});
	}

	void Track::remove_key(uint32_t idx) {
		const auto& it = std::find_if(_keys_vec.begin(), _keys_vec.end(), [idx](const KeyUniqPtr& track) {
			return track->idx == idx;
		});
		if (it != _keys_vec.end()) {
			_keys_vec.erase(it);
		}
	}

	BaseKey* Track::find_key(uint32_t idx) {
		const auto& it = std::find_if(_keys_vec.begin(), _keys_vec.end(), [idx](const KeyUniqPtr& track) {
			return track->idx == idx;
		});
		if (it != _keys_vec.end()) {
			return (*it).get();
		}
		return nullptr;
	}

	const std::vector<KeyUniqPtr>& Track::get_keys() const {
		return _keys_vec;
	}
}
