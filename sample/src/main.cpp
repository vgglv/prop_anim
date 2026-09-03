#include "nlohmann/json_fwd.hpp"
#include "prop_anim/anim.hpp"
#include "prop_anim/key.hpp"
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
				prop_anim::TrackUniqPtr track = std::make_unique<prop_anim::Track>(registry);
				std::string track_type = track_json["type"].get<std::string>();
				if (track_type == "pos") {
					auto pos_binding = std::make_unique<prop_anim::MemberBinding<GameObject, Vector2>>(*obj, &GameObject::pos);
					track->set_property_binding(std::move(pos_binding));
				}
				if (track_type == "color") {
					auto color_bind = std::make_unique<prop_anim::MemberBinding<GameObject, Color>>(*obj, &GameObject::color);
					track->set_property_binding(std::move(color_bind));
				}
				for (const auto& keys : track_json["keys"]) {
					prop_anim::KeyUniqPtr key = std::make_unique<prop_anim::AnimKey>();
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
	registry.register_type<Vector2>([](Vector2 a, Vector2 b, float t) -> Vector2 {
		Vector2 pos = a + (b - a) * t;
		return pos;
	});
	registry.register_type<Color>([](Color a, Color b, float t) -> Color {
		Color result(static_cast<uint8_t>(a.r + ((b.r - a.r) * t)), static_cast<uint8_t>(a.g + ((b.g - a.g) * t)), static_cast<uint8_t>(a.b + ((b.b - a.b) * t)),
			static_cast<uint8_t>(a.a + ((b.a - a.a) * t)));
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

	{
		GameObject* obj = addGameObject({0, 0}, {100, 100}, RED);
		nlohmann::json json = nlohmann::json::parse(json_string);
		auto anim_vector = deserialize_anim_vector(json, registry, obj);
		obj->anim_vector = std::move(anim_vector);
		obj->anim_vector->set_current_animation("default");
	}

	for (int i = 0; i < 100; i++) {
		GameObject* obj = addGameObject({0, 0}, {32, 32}, RED);
		nlohmann::json json = generate_chaotic_anim();
		auto anim_vector = deserialize_anim_vector(json, registry, obj);
		obj->anim_vector = std::move(anim_vector);
		obj->anim_vector->set_current_animation("default");
	}

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		for (const auto& obj : object_vector) {
			DrawRectangle((float)obj->pos.x, (float)obj->pos.y, (float)obj->size.x, (float)obj->size.y, obj->color);
		}
		DrawFPS(10, 10);
		EndDrawing();

		float dt = GetFrameTime();
		for (const auto& obj : object_vector) {
			if (obj->stopped) {
				continue;
			}
			if (obj->anim_vector) {
				obj->anim_vector->update(obj->current_time);
				if (auto current_anim = obj->anim_vector->current_animation()) {
					if (obj->current_time >= current_anim->get_duration()) {
						obj->current_time = 0;
						//obj->stopped = true;
					}
				}
			}
			obj->current_time += dt;
		}

		if (IsKeyPressed(KEY_D)) {
			object_vector.clear();
		}

	}

	CloseWindow();
	return 0;
}