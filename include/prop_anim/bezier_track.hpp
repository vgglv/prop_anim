#pragma once
#include "prop_anim/bezier_key.hpp"
#include "type_registry.hpp"
#include "track.hpp"
#include "property_binding.hpp"
#include <memory>

namespace prop_anim {
	class BezierTrack : public Track {
	public:
		BezierTrack();
		~BezierTrack() override = default;

		static std::unique_ptr<BezierTrack> create();
		void update(float t) override;
		void set_property_binding(std::unique_ptr<PropertyBinding> binding);
		std::unique_ptr<BaseKey> create_key(float t) override;

	private:
		std::unique_ptr<PropertyBinding> _binding = nullptr;
	};
}