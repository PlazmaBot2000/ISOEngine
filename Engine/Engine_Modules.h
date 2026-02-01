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

    float getangle() const {
        float radians = std::atan2(y, x);
        return radians * (180.0f / 3.14159265f);
    }
};


//=========================================Input=========================================
class Engine_GetAxis{
public:
    static int Y(){
		const Uint8 *keyState = SDL_GetKeyboardState(NULL);
		return std::max(-1, std::min((keyState[SDL_SCANCODE_S] - keyState[SDL_SCANCODE_W] + keyState[SDL_SCANCODE_DOWN] - keyState[SDL_SCANCODE_UP]), 1));
	}

	static int X(){
		const Uint8 *keyState = SDL_GetKeyboardState(NULL);
		return std::max(-1, std::min((keyState[SDL_SCANCODE_D] - keyState[SDL_SCANCODE_A] + keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]), 1));
	}

	static Vector2D All(){
		const Uint8 *keyState = SDL_GetKeyboardState(NULL);
		return { (double)std::max(-1, std::min((keyState[SDL_SCANCODE_D] - keyState[SDL_SCANCODE_A] + keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]), 1)), (double)std::max(-1, std::min((keyState[SDL_SCANCODE_S] - keyState[SDL_SCANCODE_W] + keyState[SDL_SCANCODE_DOWN] - keyState[SDL_SCANCODE_UP]), 1)) };
	}

};


//=======================================Colliders=======================================
class Collider {
public:
    struct CollisionResult {
        bool collided = false;
        Vector2D normal = {0, 0};
        float depth = 0;
    };
    float x, y;
    float width, height;
    
    float angle = 0.0f; 

    bool checkCollision(const Collider& b) const {
        if (angle == 0.0f && b.angle == 0.0f) {
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
        
        float angleRad = angle * DEG_TO_RAD;
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


//========================================Texture========================================
enum class ScaleMode {
    Stretch,   // Растянуть
    Fit,       // Вписать (черные полосы)
    Fill,      // Заполнить (обрезание краев)
    Repeat     // Замощение
};


class Texture {
public:
    int x, y, width, height, offsetX, offsetY = 0;
    float angle = 0.0;
    
    SDL_Texture* texture = nullptr;
    int texW = 0;
    int texH = 0;

    ScaleMode scaleMode = ScaleMode::Stretch;


    Texture() {}


    void loadTexture(SDL_Renderer* renderer, const char* filePath) {
        if (texture) SDL_DestroyTexture(texture);
        texture = IMG_LoadTexture(renderer, filePath);
        if (texture) {
            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
            if (width == 0) width = (float)texW;
            if (height == 0) height = (float)texH;
        }
    }

    ~Texture() {
        if (texture) SDL_DestroyTexture(texture);
    }

	void draw(SDL_Renderer* renderer, int camX = 0, int camY = 0) const {
	    if (!texture) return;
	
	    Uint8 oldR, oldG, oldB, oldA;
	    SDL_GetRenderDrawColor(renderer, &oldR, &oldG, &oldB, &oldA);
	
	    if (scaleMode == ScaleMode::Repeat) {
	        SDL_Texture* target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
	                                            SDL_TEXTUREACCESS_TARGET, width, height);
	        SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
	        
	        SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
	        SDL_SetRenderTarget(renderer, target);
	        
	        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
	        SDL_RenderClear(renderer);
	
	        for (int ty = 0; ty < height; ty += texH) {
	            for (int tx = 0; tx < width; tx += texW) {
	                SDL_Rect tileRect = { tx, ty, texW, texH };
	                SDL_RenderCopy(renderer, texture, NULL, &tileRect);
	            }
	        }
	
	        SDL_SetRenderTarget(renderer, oldTarget);
	        
	        SDL_Rect dest = { x + offsetX - camX, y + offsetY - camY, width, height };
	        SDL_RenderCopyEx(renderer, target, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
	        
	        SDL_DestroyTexture(target);
	    } else {
	        SDL_Rect dest = { x - camX, y - camY, width, height };
	        SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
	    }
	
	    SDL_SetRenderDrawColor(renderer, oldR, oldG, oldB, oldA);
	}


private:
    void drawRepeat(SDL_Renderer* renderer, float drawX, float drawY) const {
        SDL_Texture* target = SDL_CreateTexture(renderer, 
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)width, (int)height);
        
        if (!target) return;

        SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);

        SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, target);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
        SDL_RenderClear(renderer);

        for (int ty = 0; ty < (int)height; ty += texH) {
            for (int tx = 0; tx < (int)width; tx += texW) {
                SDL_Rect tileRect = { tx, ty, texH, texW };
                
                SDL_Rect srcRect = { 0, 0, texW, texH };
                if (tx + texW > (int)width) { tileRect.w = (int)width - tx; srcRect.w = tileRect.w; }
                if (ty + texH > (int)height) { tileRect.h = (int)height - ty; srcRect.h = tileRect.h; }

                SDL_RenderCopy(renderer, texture, &srcRect, &tileRect);
            }
        }

        SDL_SetRenderTarget(renderer, oldTarget);

        SDL_Rect finalDest = { (int)drawX, (int)drawY, (int)width, (int)height };
        SDL_RenderCopyEx(renderer, target, NULL, &finalDest, (double)angle, NULL, SDL_FLIP_NONE);

        SDL_DestroyTexture(target);
    }
};

//========================================Physics========================================
class GameObject;


enum class PhysicsType {
    Static,
    Kinematic,
    Dynamic
};


class PhysicsBody {
public:
    double x, y;
    double width, height;
    double angle;
    GameObject* parent;
    PhysicsType type = PhysicsType::Static;
    Vector2D velocity = {0.0f, 0.0f};
    Vector2D acceleration = {0.0f, 0.0f};
    float mass = 1.0f;
    float friction = 0.95f;
    float gravityScale = 1.0f;

    PhysicsBody(GameObject* p) : parent(p), x(0), y(0), width(0), height(0), angle(0) {}

    void applyForce(Vector2D force) {
        if (type == PhysicsType::Dynamic && mass > 0) {
            acceleration.x += force.x / mass;
            acceleration.y += force.y / mass;
        }
    }

    void update(const std::vector<GameObject*>& otherObjects);

private:
    void syncParentAndColliders();

    bool resolveCollisions(const std::vector<GameObject*>& otherObjects); 
};


//=======================================GameObject=======================================
class GameObject {
public:
    std::vector<Collider> colliderArray;
    PhysicsBody physics; 
    Texture texture;

    double x, y;
    double width, height;
    double angle;

    GameObject(float startX, float startY, float startW, float startH, float startAngle = 0.0f) 
    	: x(startX), y(startY), width(startW), height(startH), angle(startAngle), 
			physics(this)
	{
		physics.x = startX;
		physics.y = startY;
		physics.width = startW;
		physics.height = startH;
		physics.angle = startAngle;
	}

    GameObject() 
        : x(0), y(0), width(0), height(0), angle(0), 
          physics(this)
    {}

    void update(const std::vector<GameObject*>& otherObjects) {
        physics.update(otherObjects);
		texture.x = (int)x;    texture.x = (int)x;
    texture.y = (int)y;
    texture.width = (int)width;
    texture.height = (int)height;
    texture.angle = (float)angle;
    	texture.y = (int)y;
    	texture.angle = (float)angle;
    }

    void addCollider(float w, float h, float a) {
        Collider col;
        col.width = w;
        col.height = h;
		col.angle = a;
        colliderArray.push_back(col);
    }
};



//========================================Camera=========================================
class CinematicCamera {
public:
	GameObject * Target;
    float x = 0, y = 0;
    float lerpSpeed = 0.05f;
    float lookAhead = 50.0f;

    void update(SDL_Window *window, Vector2D Velocity = {0, 0}) {
        Vector2D Movement = Engine_GetAxis::All();
		int WindowWidth, WindowHeight;
		SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);

        float targetX = Target->x + (Target->width - WindowWidth / 2.0f) + (Velocity.x * lookAhead);
        float targetY = Target->y + (Target->height - WindowHeight / 2.0f) + (Velocity.y * lookAhead);

        x += (targetX - x) * lerpSpeed;
        y += (targetY - y) * lerpSpeed;
    }
};


bool Engine_Delay(int time, int& Prev_Tick);
int Engine_Random(int upper, int lower);
#endif
