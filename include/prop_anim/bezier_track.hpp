#pragma once
#include "track.hpp"
#include "property_binding.hpp"
#include <memory>

namespace prop_anim {
	class BezierTrack : public Track {
	public:
		BezierTrack();
		~BezierTrack() override = default;

		static std::unique_ptr<BezierTrack> create();

		static Point quad_bezier(float t, Point p0, Point p1, Point p2);
		static Point cubic_bezier(float t, Point p0, Point p1, Point p2, Point p3);
		static float evaluate_bezier(float t, Point p0, Point p1, Point p2, Point p3);

		void update(float t) override;
		void set_property_binding(std::unique_ptr<PropertyBinding> binding);

	private:
		std::unique_ptr<PropertyBinding> _binding = nullptr;
	};
}