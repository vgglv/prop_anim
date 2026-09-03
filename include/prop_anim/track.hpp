#pragma once
#include <memory>
#include <vector>
#include "key.hpp"

namespace prop_anim {
	class Track;

	using TrackUniqPtr = std::unique_ptr<Track>;
	using TrackIdx = uint32_t;
	
	class Track {
	public:
		Track();
		virtual ~Track();

		[[nodiscard]] TrackIdx idx() const;
		[[nodiscard]] float get_total_duration() const;

		void update(float t);
		void insert_sorted(KeyUniqPtr key);
		void remove_key(uint32_t idx);
		void sort();

		virtual KeyUniqPtr create_key(float t) = 0;
		virtual void setup_pose() {};
		const std::vector<KeyUniqPtr>& get_keys() const;

	private:
		TrackIdx _idx;
		std::vector<KeyUniqPtr> _keys_vec;
	};
}
