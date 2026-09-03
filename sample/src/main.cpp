#include <memory>
#include <raylib.h>
#include <vector>

namespace {
	struct GameObject {
		Vector2 pos;
		Vector2 size;
		Color color = RED;
	};
	std::vector<std::unique_ptr<GameObject>> object_vector;

	void addGameObject(Vector2 pos, Vector2 size, Color color) {
		auto obj = std::make_unique<GameObject>();
		obj->pos = pos;
		obj->size = size;
		obj->color = color;
		object_vector.emplace_back(std::move(obj));
	}
}

int main() {
	InitWindow(1280, 720, "prop_anim");

	addGameObject({0, 0}, {100, 100}, RED);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLUE);

		for (const auto& obj : object_vector) {
			DrawRectangle((float)obj->pos.x, (float)obj->pos.y, (float)obj->size.x, (float)obj->size.y, obj->color);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}