#pragma once
#include <memory>
#include <vector>
#include "key.hpp"
#include "type_registry.hpp"
#include "property_binding.hpp"

namespace prop_anim {
	class Track;

	using TrackUniqPtr = std::unique_ptr<Track>;
	using TrackIdx = uint32_t;
	
	class Track {
	public:
		Track(TypeRegistry& registry);
		virtual ~Track();

		[[nodiscard]] TrackIdx idx() const;
		[[nodiscard]] float get_total_duration() const;

		void update(float t);
		void insert_sorted(KeyUniqPtr key);
		void remove_key(uint32_t idx);
		void sort();
		void set_property_binding(std::unique_ptr<PropertyBinding> binding);

		virtual void setup_pose() {};
		const std::vector<KeyUniqPtr>& get_keys() const;

	private:
		TrackIdx _idx;
		std::vector<KeyUniqPtr> _keys_vec;
		TypeRegistry& _registry;
		std::unique_ptr<PropertyBinding> _binding;
	};
}
