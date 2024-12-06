#pragma once
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "MathHelper.h"

namespace SDLFramework {
	//Circle vs Circle
	inline bool CircleVsCircleCollision(CircleCollider* circle1, CircleCollider* circle2) {
		return (circle1->Position() - circle2->Position()).Magnitude() < (circle1->GetRadius() + circle2->GetRadius());
	}

		//Circle vs Box
	inline bool BoxVsCircleCollision(BoxCollider* box, CircleCollider* circle) {
		Vector2 circlePos = circle->Position();
		float radius = circle->GetRadius();

		Vector2 quad[4];
		quad[0] = box->GetVertexPos(0);
		quad[1] = box->GetVertexPos(1);
		quad[2] = box->GetVertexPos(2);
		quad[3] = box->GetVertexPos(3);

		//Vertex Collision Check
		for (int i = 0; i < 4; i++) {
			if ((quad[i] - circlePos).Magnitude() < radius) {
				//We have an edge of our sqaure inside our circle collider
				return true;
			}
		}

		//Square Edge collision check
		if (PointToLineDistance(quad[0], quad[1], circlePos) < radius ||
			PointToLineDistance(quad[0], quad[2], circlePos) < radius ||
			PointToLineDistance(quad[2], quad[3], circlePos) < radius ||
			PointToLineDistance(quad[3], quad[1], circlePos) < radius
			) {
			//Some point in our circle is touching one of the edges of our sqaure
			return true;
		}

		if (PointInPolygon(quad, 4, circlePos)) {
			//Our Circle is INSIDE our BoxCollider - We have collided
			return true;
		}

		return false;
	}
		//Box vs Box
	inline bool BoxVsBoxCollision(BoxCollider* box1, BoxCollider* box2) {
		Vector2 projAxes[4];
		projAxes[0] = (box1->GetVertexPos(0) - box1->GetVertexPos(1)).Normalized();
		projAxes[1] = (box1->GetVertexPos(0) - box1->GetVertexPos(2)).Normalized();
		projAxes[2] = (box2->GetVertexPos(0) - box2->GetVertexPos(1)).Normalized();
		projAxes[3] = (box2->GetVertexPos(0) - box2->GetVertexPos(2)).Normalized();

		float box1Min = 0.0f;
		float box1Max = 0.0f;
		float box2Min = 0.0f;
		float box2Max = 0.0f;
		float proj1 = 0.0f;
		float proj2 = 0.0f;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				proj1 = Dot(box1->GetVertexPos(j), projAxes[i]);
				proj2 = Dot(box2->GetVertexPos(j), projAxes[i]);

				if (j == 0) {
					box1Min = box1Max = proj1;
				}
				else {
					if (proj1 < box1Min) {
						box1Min = proj1;
					}
					if (proj1 > box1Max) {
						box1Max = proj1;
					}

					if (proj2 < box2Min) {
						box2Min = proj2;
					}
					if (proj2 > box2Max) {
						box2Max = proj2;
					}
				}
			}

			//At this point, we have our two box projections
			float halfDist1 = (box1Max - box1Min) * 0.5f;
			float midPoint1 = box1Min + halfDist1;

			float halfDist2 = (box2Max - box2Min) * 0.5f;
			float midPoint2 = box2Min + halfDist2;

			if (abs(midPoint1 - midPoint2) > (halfDist1 + halfDist2)) {
				return false;
			}
		}

		//Implicitly returns true
	}

	inline bool ColliderVsColliderCheck(Collider* collider1, Collider* collider2) {
		if (collider1->GetType() == Collider::ColliderType::Circle &&
			collider2->GetType() == Collider::ColliderType::Circle) {
			//We want to call CircleVsCircle collision
			return CircleVsCircleCollision(static_cast<CircleCollider*>(collider1), static_cast<CircleCollider*>(collider2));
		} 
		else if (collider1->GetType() == Collider::ColliderType::Box &&
			collider2->GetType() == Collider::ColliderType::Circle) {
			return BoxVsCircleCollision(static_cast<BoxCollider*>(collider1), static_cast<CircleCollider*>(collider2));
		}
		else if (collider1->GetType() == Collider::ColliderType::Circle &&
			collider2->GetType() == Collider::ColliderType::Box) {
			return BoxVsCircleCollision(static_cast<BoxCollider*>(collider2), static_cast<CircleCollider*>(collider1));
		}
		else if (collider1->GetType() == Collider::ColliderType::Box &&
			collider2->GetType() == Collider::ColliderType::Box) {
			return BoxVsBoxCollision(static_cast<BoxCollider*>(collider1), static_cast<BoxCollider*>(collider2));
		}
		else {
			return false;
		}
	}
}