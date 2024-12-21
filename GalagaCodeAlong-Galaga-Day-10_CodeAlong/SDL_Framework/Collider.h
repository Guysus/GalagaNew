#pragma once
#include "Texture.h"

namespace SDLFramework {

	//Abstract class
	class Collider : public GameEntity {
	public:
		enum class ColliderType {Box, Circle};

		Collider(ColliderType type);
		virtual ~Collider();

		virtual Vector2 GetFurthestPoint() = 0;

		ColliderType GetType();

		virtual void Render() override;//added the override
	protected:
		ColliderType mType;

		static const bool DEBUG_COLLIDERS = true;
		Texture* mDebugTexture;

		void SetDebugTexture(Texture* texture);
	};
}