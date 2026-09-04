#pragma once
#include <vector>
#include <string>
#include "track.hpp"
#include <string_view>

namespace prop_anim {
	class Anim;
	using AnimUniqPtr = std::unique_ptr<Anim>;

	class Anim {
	public:
		explicit Anim(std::string name);
		static AnimUniqPtr create(const std::string& name);

		void update(float t);
		void add_track(TrackUniqPtr track);
		void remove_track(TrackIdx idx);
		[[nodiscard]] float get_duration() const;
		Track* find_track(TrackIdx idx);
		[[nodiscard]] const std::string& name() const;
		const std::vector<TrackUniqPtr>& get_tracks() const;
	
	private:
		std::vector<TrackUniqPtr> _tracks_vec;
		std::string _name;
	};
}
