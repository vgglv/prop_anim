#include <memory>
#include <prop_anim/anim.hpp>
#include <algorithm>
#include <ranges>
#include <utility>

namespace prop_anim {
	AnimUniqPtr Anim::create(const std::string &name) {
		auto result = std::make_unique<Anim>(name);
		return result;
	}

	Anim::Anim(std::string name) : _name(std::move(name)) {}

	void Anim::update(float t) {
		for (const auto& track : _tracks_vec) {
			track->update(t);
		}
	}

	void Anim::add_track(TrackUniqPtr track) {
		_tracks_vec.emplace_back(std::move(track));
	}

	void Anim::remove_track(TrackIdx idx) {
		const auto& it = std::ranges::find_if(_tracks_vec, [idx](const TrackUniqPtr& track) {
			return track->idx() == idx;
		});
		if (it != _tracks_vec.end()) {
			_tracks_vec.erase(it);
		}
	}

	Track* Anim::find_track(TrackIdx idx) {
		const auto& it = std::ranges::find_if(_tracks_vec, [idx](const TrackUniqPtr& track) {
			return track->idx() == idx;
		});
		if (it != _tracks_vec.end()) {
			return (*it).get();
		}

		return nullptr;
	}

	std::string_view Anim::name() const {
		return _name;
	}

	const std::vector<TrackUniqPtr>& Anim::get_tracks() const {
		return _tracks_vec;
	}
}
