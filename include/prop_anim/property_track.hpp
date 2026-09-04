#pragma once
#include "type_registry.hpp"
#include "track.hpp"
#include "property_binding.hpp"
#include <memory>

namespace prop_anim {
	class PropertyTrack;
	using PropertyTrackUniqPtr = std::unique_ptr<PropertyTrack>;

	class PropertyTrack : public Track {
	public:
		PropertyTrack(TypeRegistry& registry);
		~PropertyTrack() override = default;

		void update(float t) override;
		void set_property_binding(std::unique_ptr<PropertyBinding> binding);

	private:
		TypeRegistry& _registry;
		std::unique_ptr<PropertyBinding> _binding = nullptr;
	};
}