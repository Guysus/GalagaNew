#include "Collider.h"

namespace SDLFramework {
	Collider::Collider(ColliderType type) : mType(type) {
		mDebugTexture = nullptr;
	}

	Collider::~Collider() {
		delete mDebugTexture;
		mDebugTexture = nullptr;
	}

	void Collider::Render() {
		if (DEBUG_COLLIDERS) {
			mDebugTexture->Render();
		}
	}

	Collider::ColliderType Collider::GetType() {
		return mType;
	}

	void Collider::SetDebugTexture(Texture* tex) {
		delete mDebugTexture;
		mDebugTexture = tex;
		mDebugTexture->Parent(this);
	}
}