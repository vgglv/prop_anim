#include "property_binding.hpp"

namespace prop_anim {
	template<typename Object, typename Value>
	class MemberBinding : public PropertyBinding {
	public:
		using Member = Value Object::*;

		MemberBinding(Object& object, Member member) : _object(&object), _member(member) {}

		void set(const AnyValue& value) override {
			_object->*_member = std::any_cast<Value>(value);
		}

	private:
		Object* _object;
		Member _member;
	};
}