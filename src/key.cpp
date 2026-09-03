#include "prop_anim/key.hpp"

namespace {
	uint32_t last_index = 0;
}

namespace prop_anim {
	AnimKey::AnimKey() : idx(++last_index) {}
}
