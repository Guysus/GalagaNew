#include "PhysEntity.h"
#include "PhysicsManager.h"
#include "PhysicsHelper.h"

namespace SDLFramework {
	void PhysEntity::AddCollider(Collider* collider, Vector2 localPos) {
		collider->Parent(this);
		collider->Position(localPos);
		mColliders.push_back(collider);

		if (mColliders.size() > 1 || mColliders[0]->GetType() != Collider::ColliderType::Circle) {
			float furthestDistance = mColliders[0]->GetFurthestPoint().Magnitude();
			float dist = 0.0f;

			for (int i = 1; i < mColliders.size(); i++) {
				dist = mColliders[i]->GetFurthestPoint().Magnitude();

				if (dist > furthestDistance) {
					furthestDistance = dist;
				}
			}

			delete mBroadPhaseCollider;
			mBroadPhaseCollider = new CircleCollider(furthestDistance, true);
			mBroadPhaseCollider->Parent(this);
			mBroadPhaseCollider->Position(Vec2_Zero);
		}
	}

	PhysEntity::PhysEntity() {
		mBroadPhaseCollider = nullptr;
		mId = 0;
	}

	PhysEntity::~PhysEntity() {
		for (auto colliders : mColliders) {
			delete colliders;
			colliders = nullptr;
		}
		mColliders.clear();

		delete mBroadPhaseCollider;
		mBroadPhaseCollider = nullptr;

		if (mId != 0) {
			PhysicsManager::Instance()->UnregisterEntity(mId);
		}
	}

	unsigned long PhysEntity::GetId() {
		return mId;
	}

	bool PhysEntity::IgnoreCollisions() {
		return false;
	}

	void PhysEntity::Render() {
		for (auto colliders : mColliders) {
			colliders->Render();
		}

		if (mBroadPhaseCollider) {
			mBroadPhaseCollider->Render();
		}
	}

	bool PhysEntity::CheckCollision(PhysEntity* other) {
		if (IgnoreCollisions() || other->IgnoreCollisions()) {
			//returning early because at least one of us is ignoring collisions rn
			return false;
		}

		bool narrowPhaseCheck = false;

		if (mBroadPhaseCollider && other->mBroadPhaseCollider) {
			narrowPhaseCheck = ColliderVsColliderCheck(mBroadPhaseCollider, other->mBroadPhaseCollider);
		}

		if (narrowPhaseCheck) {
			for (int i = 0; i < mColliders.size(); i++) {
				for (int j = 0; j < other->mColliders.size(); j++) {
					if (ColliderVsColliderCheck(mColliders[i], other->mColliders[j])) {
						//A collision has happened!
						return true;
					}
				}
			}
		}

		return false;
	}
}