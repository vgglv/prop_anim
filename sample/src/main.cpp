#include "nlohmann/json_fwd.hpp"
#include "prop_anim/anim.hpp"
#include "prop_anim/bezier_key.hpp"
#include "prop_anim/bezier_track.hpp"
#include "prop_anim/key.hpp"
#include "prop_anim/property_track.hpp"
#include "prop_anim/track.hpp"
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <prop_anim/type_registry.hpp>
#include <prop_anim/anim_vector.hpp>
#include <prop_anim/member_binding.hpp>
#include <nlohmann/json.hpp>

namespace {
	struct GameObject {
		Vector2 pos;
		Vector2 size;
		Color color = RED;
		std::unique_ptr<prop_anim::AnimVector> anim_vector;
		float current_time = 0.f;
		bool stopped = false;
	};
	std::vector<std::unique_ptr<GameObject>> object_vector;

	std::unique_ptr<prop_anim::AnimVector> deserialize_anim_vector(const nlohmann::json& json, prop_anim::TypeRegistry& registry, GameObject* obj) {
		std::unique_ptr<prop_anim::AnimVector> result = std::make_unique<prop_anim::AnimVector>();
		for (const auto& anim_json : json["animations"]) {
			const std::string name = anim_json["name"].get<std::string>();
			prop_anim::AnimUniqPtr anim = std::make_unique<prop_anim::Anim>(name);
			for (const auto& track_json : anim_json["tracks"]) {
				prop_anim::PropertyTrackUniqPtr track = std::make_unique<prop_anim::PropertyTrack>(registry);
				std::string track_type = track_json["type"].get<std::string>();
				if (track_type == "pos") {
					auto pos_binding = std::make_unique<prop_anim::MemberBinding<GameObject, Vector2>>("Vector2", *obj, &GameObject::pos);
					track->set_property_binding(std::move(pos_binding));
				}
				if (track_type == "color") {
					auto color_bind = std::make_unique<prop_anim::MemberBinding<GameObject, Color>>("Color", *obj, &GameObject::color);
					track->set_property_binding(std::move(color_bind));
				}
				for (const auto& keys : track_json["keys"]) {
					prop_anim::KeyUniqPtr key = std::make_unique<prop_anim::BaseKey>();
					key->time = keys["time"].get<float>();
					if (track_type == "pos") {
						key->value = Vector2{keys["value"][0].get<float>(), keys["value"][1].get<float>()};
					}
					if (track_type == "color") {
						key->value = Color{keys["value"][0].get<uint8_t>(), keys["value"][1].get<uint8_t>(), keys["value"][2].get<uint8_t>(), keys["value"][3].get<uint8_t>()};
					}
					track->insert_sorted(std::move(key));
				}
				anim->add_track(std::move(track));
			}
			result->add_animation(std::move(anim));
		}
		return result;
	};

	nlohmann::json generate_chaotic_anim() {
		nlohmann::json result = nlohmann::json::object();
		nlohmann::json animations = nlohmann::json::array();
		nlohmann::json tracks = nlohmann::json::array();

		{
			nlohmann::json track_pos = nlohmann::json::object();
			track_pos["type"] = "pos";

			nlohmann::json pos1 = nlohmann::json::array({GetRandomValue(0, 1280), GetRandomValue(0, 720)});
			nlohmann::json pos2 = nlohmann::json::array({GetRandomValue(0, 1280), GetRandomValue(0, 720)});
			nlohmann::json pos3 = nlohmann::json::array({GetRandomValue(0, 1280), GetRandomValue(0, 720)});
			nlohmann::json pos4 = nlohmann::json::array({GetRandomValue(0, 1280), GetRandomValue(0, 720)});

			nlohmann::json key1 = nlohmann::json::object({{"time", 0.f}, {"value", pos1}});
			nlohmann::json key2 = nlohmann::json::object({{"time", 1.f}, {"value", pos2}});
			nlohmann::json key3 = nlohmann::json::object({{"time", 2.f}, {"value", pos3}});
			nlohmann::json key4 = nlohmann::json::object({{"time", 3.f}, {"value", pos4}});
			nlohmann::json key5 = nlohmann::json::object({{"time", 4.f}, {"value", pos1}});

			nlohmann::json keys = nlohmann::json::array({key1, key2 ,key3, key4, key5});

			track_pos["keys"] = keys;
			tracks.emplace_back(track_pos);
		}

		{
			nlohmann::json color_pos = nlohmann::json::object();
			color_pos["type"] = "color";

			nlohmann::json color1 = nlohmann::json::array({GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255});
			nlohmann::json color2 = nlohmann::json::array({GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255});

			nlohmann::json key1 = nlohmann::json::object({{"time", 0.f}, {"value", color1}});
			nlohmann::json key2 = nlohmann::json::object({{"time", 2.f}, {"value", color2}});
			nlohmann::json key3 = nlohmann::json::object({{"time", 4.f}, {"value", color1}});
			nlohmann::json keys = nlohmann::json::array({key1, key2 ,key3});

			color_pos["keys"] = keys;
			tracks.emplace_back(color_pos);
		}

		nlohmann::json anim = nlohmann::json::object();
		anim["name"] = "default";
		anim["tracks"] = tracks;

		animations.emplace_back(anim);
		result["animations"] = animations;
		return result;
	}

	GameObject* addGameObject(Vector2 pos, Vector2 size, Color color) {
		auto obj = std::make_unique<GameObject>();
		auto result = obj.get();
		obj->pos = pos;
		obj->size = size;
		obj->color = color;
		object_vector.emplace_back(std::move(obj));
		return result;
	}

}

int main() {
	InitWindow(1280, 720, "prop_anim");
	SetTargetFPS(60);
	SetRandomSeed(std::time(NULL));

	prop_anim::TypeRegistry registry;
	registry.register_type<Vector2>("Vector2", [](Vector2 a, Vector2 b, float t) -> Vector2 {
		Vector2 pos = a + (b - a) * t;
		return pos;
	});
	registry.register_type<Color>("Color", [](Color a, Color b, float t) -> Color {
		Color result;
		result.r = static_cast<uint8_t>(a.r + ((b.r - a.r) * t));
		result.g = static_cast<uint8_t>(a.g + ((b.g - a.g) * t));
		result.b = static_cast<uint8_t>(a.b + ((b.b - a.b) * t));
		result.a = static_cast<uint8_t>(a.a + ((b.a - a.a) * t));
		return result;
	});
	std::string_view json_string = R"(
		{
			"animations": [
				{
					"name": "default",
					"tracks": [
						{
							"type": "pos",
							"keys": [
								{
									"value": [0.0, 0.0],
									"time": 0.0
								},
								{
									"value": [1280.0, 720.0],
									"time": 1.0
								},
								{
									"value": [0.0, 0.0],
									"time": 2.0
								}
							]
						},

						{
							"type": "color",
							"keys": [
								{
									"value": [255, 255, 255, 255],
									"time": 0.0
								},
								{
									"value": [255, 255, 0, 255],
									"time": 1.0
								},
								{
									"value": [255, 255, 255, 255],
									"time": 2.0
								}
							]
						}
					]
				}
			]	
		}	
	)";

//	{
//		GameObject* obj = addGameObject({0, 0}, {100, 100}, RED);
//		nlohmann::json json = nlohmann::json::parse(json_string);
//		auto anim_vector = deserialize_anim_vector(json, registry, obj);
//		obj->anim_vector = std::move(anim_vector);
//		obj->anim_vector->set_current_animation("default");
//	}
//
//	for (int i = 0; i < 100; i++) {
//		GameObject* obj = addGameObject({0, 0}, {32, 32}, RED);
//		nlohmann::json json = generate_chaotic_anim();
//		auto anim_vector = deserialize_anim_vector(json, registry, obj);
//		obj->anim_vector = std::move(anim_vector);
//		obj->anim_vector->set_current_animation("default");
//	}
	{
		GameObject* obj = addGameObject({0, 0}, {100, 100}, RED);
		auto anim_vector = prop_anim::AnimVector::create();
		auto anim = prop_anim::Anim::create("default");
		{ //x prop
			auto bezier_track_x = prop_anim::BezierTrack::create();
			auto x_binding = std::make_unique<prop_anim::MemberBinding<Vector2, float>>("float", obj->pos, &Vector2::x);
			bezier_track_x->set_property_binding(std::move(x_binding));
			auto key1 = bezier_track_x->create_key(0.0f);
			auto bezier_key1 = static_cast<prop_anim::BezierKey*>(key1.get());
			bezier_key1->value = 0.f;
			bezier_key1->in_handle = {-0.25f, 300.0f};
			bezier_key1->out_handle = {1.f, -300.0f};
			bezier_track_x->insert_sorted(std::move(key1));

			auto key2 = bezier_track_x->create_key(2.0f);
			auto bezier_key2 = static_cast<prop_anim::BezierKey*>(key2.get());
			bezier_key2->value = 1280.f - 100.f;
			bezier_key2->in_handle = {-0.25, 0.0f};
			bezier_key2->out_handle = {1.f, 300.0f};
			bezier_track_x->insert_sorted(std::move(key2));

			auto key3 = bezier_track_x->create_key(4.0f);
			auto bezier_key3 = static_cast<prop_anim::BezierKey*>(key3.get());
			bezier_key3->value = 0.f;
			bezier_key3->in_handle = {-0.25, 0.0f};
			bezier_key3->out_handle = {0.25f, 0.0f};
			bezier_track_x->insert_sorted(std::move(key3));

			anim->add_track(std::move(bezier_track_x));
		}

		{ //y prop
			auto bezier_track_y = prop_anim::BezierTrack::create();
			auto y_binding = std::make_unique<prop_anim::MemberBinding<Vector2, float>>("float", obj->pos, &Vector2::y);
			bezier_track_y->set_property_binding(std::move(y_binding));
			auto key1 = bezier_track_y->create_key(0.0f);
			auto bezier_key1 = static_cast<prop_anim::BezierKey*>(key1.get());
			bezier_key1->value = 0.f;
			bezier_key1->in_handle = {-0.25, 0.0f};
			bezier_key1->out_handle = {0.25f, 0.0f};
			bezier_track_y->insert_sorted(std::move(key1));

			auto key2 = bezier_track_y->create_key(2.0f);
			auto bezier_key2 = static_cast<prop_anim::BezierKey*>(key2.get());
			bezier_key2->value = 720.f - 100.f;
			bezier_key2->in_handle = {-0.25, 0.0f};
			bezier_key2->out_handle = {0.25f, 0.0f};
			bezier_track_y->insert_sorted(std::move(key2));

			auto key3 = bezier_track_y->create_key(4.0f);
			auto bezier_key3 = static_cast<prop_anim::BezierKey*>(key3.get());
			bezier_key3->value = 0.f;
			bezier_key3->in_handle = {-0.25, 0.0f};
			bezier_key3->out_handle = {0.25f, 0.0f};
			bezier_track_y->insert_sorted(std::move(key3));

			anim->add_track(std::move(bezier_track_y));
		}
		anim_vector->add_animation(std::move(anim));
		obj->anim_vector = std::move(anim_vector);
		obj->anim_vector->set_current_animation("default");
	}

	float slow_multiplier = 1.f;

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		for (const auto& obj : object_vector) {
			DrawRectangle((float)obj->pos.x, (float)obj->pos.y, (float)obj->size.x, (float)obj->size.y, obj->color);
		}
		DrawFPS(10, 10);
		EndDrawing();

		if (IsKeyPressed(KEY_MINUS)) {
			slow_multiplier += .5f;
		}
		if (IsKeyPressed(KEY_EQUAL)) {
			slow_multiplier -= .5f;
		}

		float dt = GetFrameTime() / slow_multiplier;
		for (const auto& obj : object_vector) {
			if (obj->stopped) {
				continue;
			}
			obj->current_time += dt;
			if (obj->anim_vector) {
				obj->anim_vector->update(obj->current_time);
				if (auto current_anim = obj->anim_vector->current_animation()) {
					if (obj->current_time >= current_anim->get_duration()) {
						obj->current_time = 0;
						//obj->stopped = true;
					}
				}
			}
		}

		if (IsKeyPressed(KEY_D)) {
			object_vector.clear();
		}

	}

	CloseWindow();
	return 0;
}