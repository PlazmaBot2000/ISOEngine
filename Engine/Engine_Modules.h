#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

#ifndef MODULES_H
#define MODULES_H

//=======================================Vector2D========================================
class Vector2D {
	public:
    	double x, y;
    	Vector2D operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }
    	Vector2D operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }
		Vector2D operator*(double scalar) const { return {x * scalar, y * scalar}; }
    	double magnitude() const { return std::sqrt(x * x + y * y); }

		float getAngleDegrees() const {
        	float radians = std::atan2(y, x);
        	return radians * (180.0f / 3.14159265f);
    	}
};

//=========================================Input=========================================
class Engine_GetAxis{
	public:
		static int X();
		static int Y();
		static Vector2D All();
};

//=======================================Colliders=======================================
struct CollisionResult {
    bool collided = false;
    Vector2D normal = {0, 0};
    float depth = 0;
};

class Collider {
public:
    float x, y;
    float width, height;
    
    float angleDegrees = 0.0f; 

    bool checkCollision(const Collider& b) const {
        if (angleDegrees == 0.0f && b.angleDegrees == 0.0f) {
            return (x < b.x + b.width &&
                    x + width > b.x &&
                    y < b.y + b.height &&
                    y + height > b.y);
        }
        return checkCollisionSAT(b);
    }

    void draw(SDL_Renderer* renderer) const {
        auto vertices = getVertices();
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        for (int i = 0; i < 4; i++) {
            const Vector2D& p1 = vertices[i];
            const Vector2D& p2 = vertices[(i + 1) % 4];
            SDL_RenderDrawLine(renderer, 
                static_cast<int>(p1.x), static_cast<int>(p1.y), 
                static_cast<int>(p2.x), static_cast<int>(p2.y));
        }
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }

		CollisionResult getCollisionData(const Collider& b) const {
    	auto v1 = getVertices();
    	auto v2 = b.getVertices();
    	CollisionResult result;
    	result.collided = true;
    	float minOverlap = 1e10f;

    	auto checkAxis = [&](const std::vector<Vector2D>& r1, const std::vector<Vector2D>& r2) {
        	for (int i = 0; i < 4; i++) {
            	Vector2D p1 = r1[i];
            	Vector2D p2 = r1[(i + 1) % 4];
            	Vector2D axis = {-(p2.y - p1.y), p2.x - p1.x};
            
            	float len = axis.magnitude();
            	axis.x /= len; axis.y /= len;

            	auto getMinMax = [&](const std::vector<Vector2D>& r, float& min, float& max) {
                	min = max = (r[0].x * axis.x + r[0].y * axis.y);
                	for (const auto& p : r) {
                    	float proj = (p.x * axis.x + p.y * axis.y);
                    	if (proj < min) min = proj;
                    	if (proj > max) max = proj;
                	}
            	};

            	float min1, max1, min2, max2;
            	getMinMax(r1, min1, max1);
            	getMinMax(r2, min2, max2);

            	if (max1 < min2 || max2 < min1) return false;

            	float overlap = std::min(max1, max2) - std::max(min1, min2);
            	if (overlap < minOverlap) {
                	minOverlap = overlap;
                	result.normal = axis;
                	result.depth = overlap;
            	}
        	}
        	return true;
    	};

    	if (!checkAxis(v1, v2) || !checkAxis(v2, v1)) {
        	return {false, {0,0}, 0};
    	}

    	Vector2D centerA = {x + width/2, y + height/2};
    	Vector2D centerB = {b.x + b.width/2, b.y + b.height/2};
    	Vector2D dir = centerA - centerB;
    	if ((dir.x * result.normal.x + dir.y * result.normal.y) < 0) {
        	result.normal = result.normal * -1.0;
    	}

    	return result;
	}

private:
    const float DEG_TO_RAD = 3.14159265f / 180.0f;

    std::vector<Vector2D> getVertices() const {
        float cx = x + width / 2.0f;
        float cy = y + height / 2.0f;
        
        float angleRad = angleDegrees * DEG_TO_RAD;
        float cosA = cosf(angleRad);
        float sinA = sinf(angleRad);

        std::vector<Vector2D> vertices(4);
        float dx[4] = {-width/2, width/2, width/2, -width/2};
        float dy[4] = {-height/2, -height/2, height/2, height/2};

        for (int i = 0; i < 4; i++) {
            vertices[i].x = cx + dx[i] * cosA - dy[i] * sinA;
            vertices[i].y = cy + dx[i] * sinA + dy[i] * cosA;
        }
        return vertices;
    }

    bool checkCollisionSAT(const Collider& b) const {
        auto v1 = getVertices();
        auto v2 = b.getVertices();

        auto checkAxis = [&](const std::vector<Vector2D>& r1, const std::vector<Vector2D>& r2) {
            for (int i = 0; i < 4; i++) {
                Vector2D p1 = r1[i];
                Vector2D p2 = r1[(i + 1) % 4];
                Vector2D axis = {-(p2.y - p1.y), p2.x - p1.x};

                auto getMinMax = [&](const std::vector<Vector2D>& r, float& min, float& max) {
                    min = max = (r[0].x * axis.x + r[0].y * axis.y);
                    for (const auto& p : r) {
                        float proj = (p.x * axis.x + p.y * axis.y);
                        if (proj < min) min = proj;
                        if (proj > max) max = proj;
                    }
                };

                float min1, max1, min2, max2;
                getMinMax(r1, min1, max1);
                getMinMax(r2, min2, max2);

                if (max1 < min2 || max2 < min1) return false;
            }
            return true;
        };

        return checkAxis(v1, v2) && checkAxis(v2, v1);
    }
};

//=======================================GameObject======================================
enum class ScaleMode {
    Stretch,   // Растянуть (игнорируя пропорции)
    Fit,       // Вписать (с сохранением пропорций, могут быть пустые поля)
    Fill,      // Заполнить (с сохранением пропорций, часть картинки может выйти за границы)
	Repeat     // Замощение (с повторением изображения)
};


enum class PhysicsType {
	Static,		// Объект недвижим
	Kinematic,	// Объект можно двигать но он не падает
	Dynamic		// На объект действую внешние физические силы
};

class PhysicsClass{
public:
	PhysicsType physicsType = PhysicsType::Static;
    Vector2D velocity = {0, 0};
    Vector2D acceleration = {0, 0};
    float mass = 1.0f;
    float friction = 0.95f;
    float gravityScale = 1.0f;
};

class GameObject {
public:
    Collider collider;
    
	PhysicsClass Physics;

	double x, y;
	double height, width;
        
    SDL_Texture* texture = nullptr;
    int texW = 0, texH = 0;
    ScaleMode scaleMode = ScaleMode::Stretch;

    void loadTexture(SDL_Renderer* renderer, const char* filePath) {
        texture = IMG_LoadTexture(renderer, filePath);
        if (texture) {
            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
            width = collider.width = (float)texW;
            height = collider.height = (float)texH;
        }
    }

    void applyForce(Vector2D force) {
        if (Physics.physicsType == PhysicsType::Dynamic && Physics.mass > 0) {
            Physics.acceleration.x += force.x / Physics.mass;
            Physics.acceleration.y += force.y / Physics.mass;
        }
    }

    void updatePosition(const std::vector<GameObject*>& otherObjects) {
        if (Physics.physicsType == PhysicsType::Static) return;

        if (Physics.physicsType == PhysicsType::Dynamic) {
            applyForce({0, 0.5f * Physics.gravityScale});

            Physics.velocity.x += Physics.acceleration.x;
            Physics.velocity.y += Physics.acceleration.y;
            
            Physics.velocity.x *= Physics.friction;
            Physics.velocity.y *= Physics.friction;

            Physics.acceleration = {0, 0};
        }

        collider.x += Physics.velocity.x;
        resolveCollisions(otherObjects, true);

        collider.y += Physics.velocity.y;
        resolveCollisions(otherObjects, false);
    }

    void draw(SDL_Renderer* renderer) const {
    if (!texture) return;

    Uint8 oldR, oldG, oldB, oldA;
    SDL_GetRenderDrawColor(renderer, &oldR, &oldG, &oldB, &oldA);

    if (scaleMode == ScaleMode::Repeat) {
        SDL_Texture* target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
                                              SDL_TEXTUREACCESS_TARGET, (int)collider.width, (int)collider.height);
        SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
        
        SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, target);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
        SDL_RenderClear(renderer);

        for (int ty = 0; ty < (int)collider.height; ty += texH) {
            for (int tx = 0; tx < (int)collider.width; tx += texW) {
                SDL_Rect tileRect = { tx, ty, texW, texH };
                SDL_RenderCopy(renderer, texture, NULL, &tileRect);
            }
        }

        SDL_SetRenderTarget(renderer, oldTarget);
        
        SDL_Rect dest = { (int)collider.x, (int)collider.y, (int)collider.width, (int)collider.height };
        SDL_RenderCopyEx(renderer, target, NULL, &dest, collider.angleDegrees, NULL, SDL_FLIP_NONE);
        
        SDL_DestroyTexture(target);
    } else {
        SDL_Rect dest = { (int)collider.x, (int)collider.y, (int)collider.width, (int)collider.height };
        SDL_RenderCopyEx(renderer, texture, NULL, &dest, collider.angleDegrees, NULL, SDL_FLIP_NONE);
    }

    SDL_SetRenderDrawColor(renderer, oldR, oldG, oldB, oldA);
}



private:
    void resolveCollisions(const std::vector<GameObject*>& otherObjects, bool isXAxis) {
        for (const auto& other : otherObjects) {
            if (other == this) continue;

            CollisionResult res = collider.getCollisionData(other->collider);
            if (res.collided) {
                collider.x += res.normal.x * res.depth;
                collider.y += res.normal.y * res.depth;

                if (isXAxis) Physics.velocity.x = 0;
                else Physics.velocity.y = 0;
            }
        }
    }
};


bool Engine_Delay(int time, int& Prev_Tick);
int Engine_Random(int upper, int lower);
#endif
