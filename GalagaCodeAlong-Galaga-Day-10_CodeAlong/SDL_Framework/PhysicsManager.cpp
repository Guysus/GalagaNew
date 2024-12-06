#include "PhysicsManager.h"

namespace SDLFramework {
	PhysicsManager* PhysicsManager::sInstance = nullptr;

	PhysicsManager* PhysicsManager::Instance() {
		if (sInstance == nullptr) {
			sInstance = new PhysicsManager();
		}

		return sInstance;
	}

	void PhysicsManager::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	PhysicsManager::PhysicsManager() {
		mLastID = 0;

		for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++) {
			mLayerMasks[i] = std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>(static_cast<unsigned int>(CollisionFlags::None));
		}
	}

	PhysicsManager::~PhysicsManager() {
		for (auto layer : mCollisionLayers) {
			layer.clear();
		}
	}

	void PhysicsManager::SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flag) {
		mLayerMasks[static_cast<unsigned int>(layer)] =
			std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>(static_cast<unsigned int>(flag));
	}

	unsigned long PhysicsManager::RegisterEntity(PhysEntity* entity, CollisionLayers layer) {
		mCollisionLayers[static_cast<unsigned int>(layer)].push_back(entity);
		mLastID++;
		return mLastID;
	}

	void PhysicsManager::UnregisterEntity(unsigned long id) {
		bool found = false;

		//Loop through all of our collision layers
		for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers) && !found; i++) {
			//Loop through all of the OBJECTS in a specific collision layer
			for (int j = 0; j < mCollisionLayers[i].size() && !found; j++) {
				//i == the layer we are currently looking at
				//j == the PhysEntity* we are currently looking at
				if (mCollisionLayers[i][j]->GetId() == id) {
					mCollisionLayers[i].erase(mCollisionLayers[i].begin() + j);
					found = true;
				}
			}
		}
	}

	void PhysicsManager::Update() {
		//Iteration 1 - Looking through all of our layers
		for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++) {
			//Iteration 2 - Looking through all of our layers
			for (int j = 0; j < static_cast<unsigned int>(CollisionLayers::MaxLayers); j++) {
				//Checking to make sure these layers CAN register collision
				//AND that we haven't already checked these two against each other
				if (mLayerMasks[i].test(j) && i <= j) {
					//Iteration 1 - Looking through all of our objects on iteration 1's layer
					for (int k = 0; k < mCollisionLayers[i].size(); k++) {
						//Iteration 2 - Looking through all of our objects on iterations 2's layer
						for (int l = 0; l < mCollisionLayers[j].size(); l++) {
							//This is where the actual collision check is happening!
							if (mCollisionLayers[i][k]->CheckCollision(mCollisionLayers[j][l])) {
								//A collision has happened!
								//Our first object has hit our second object
								mCollisionLayers[i][k]->Hit(mCollisionLayers[j][l]);
								//Our second object has hit our first object
								mCollisionLayers[j][l]->Hit(mCollisionLayers[i][k]);
							}
						}
					}
				}
			}
		}
	}
}