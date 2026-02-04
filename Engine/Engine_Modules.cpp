#include <SDL2/SDL.h>
#include "Engine_Modules.h"


//Delay для использования в Update
bool Engine_Delay(int time, int& Prev_Tick){
	int diff = SDL_GetTicks() - Prev_Tick;
	if(diff >= time){
		Prev_Tick = SDL_GetTicks();
		return true;
	}
	return false;
}
//Псевдослучайное число в интервале
int Engine_Random(int upper, int lower){
	return (rand() % (upper-lower) + 1);
}

//=======================================Physics======================================
void PhysicsBody::syncParentAndColliders() {
    if (!parent) return;
    
    parent->x = x;
    parent->y = y;
    parent->angle = angle;

    for (auto& col : parent->colliderArray) {
        col.x = (float)x;
        col.y = (float)y;
        col.angle = (float)angle;
    }
}


bool PhysicsBody::resolveCollisions(const std::vector<GameObject*>& otherObjects) {
	bool hadCollision = false;

	for (const auto& other : otherObjects) {
		if (other == parent) continue;

		for (auto& myCol : parent->colliderArray) {
			for (const auto& otherCol : other->colliderArray) {
				Collider::CollisionResult res = myCol.getCollisionData(otherCol);
				if (res.collided) {
					hadCollision = true;
					x += res.normal.x * res.depth;
					y += res.normal.y * res.depth;
					syncParentAndColliders();
				}
			}
		}
	}
	return hadCollision;
}


void PhysicsBody::update(const std::vector<GameObject*>& otherObjects) {
	x = parent->x;
	y = parent->y;
	angle = parent->angle;
	width = parent->width;
	height = parent->height;

	if (type == PhysicsType::Static) {
		syncParentAndColliders(); 
		return;
	}

	if (type == PhysicsType::Dynamic) {
		applyForce({0, 0.5f * (float)gravityScale});
		velocity.x += acceleration.x;
		velocity.y += acceleration.y;
		velocity.x *= friction;
		velocity.y *= friction;
		acceleration = {0, 0};
	}

	x += velocity.x;
	syncParentAndColliders();
	if (resolveCollisions(otherObjects)) {
		velocity.x = 0;
	}

	y += velocity.y;
	syncParentAndColliders();
	if (resolveCollisions(otherObjects)) {
		velocity.y = 0;
	}
}


void ISODraw_RowLeft(float startX, float startY, SDL_Renderer* renderer, const std::vector<int>& row_map, const std::unordered_map<int, Tile*>& tile_library, float camX, float camY) {
	int lastX = startX, lastY = startY;
	for (int i = 0; i < row_map.size(); i++){
		int tileID = row_map[i];
            if (tile_library.find(tileID) != tile_library.end() && tile_library.at(tileID) != nullptr) {
                Tile* currentTile = tile_library.at(tileID);
                lastX = currentTile->texture.x = lastX - (currentTile->offsetX * currentTile->scale);
                lastY = currentTile->texture.y = lastY + (currentTile->offsetY * currentTile->scale);                
                currentTile->texture.draw(renderer, camX, camY);
            }
	}
}
void ISODraw_RowRight(float startX, float startY, SDL_Renderer* renderer, const std::vector<int>& row_map, const std::unordered_map<int, Tile*>& tile_library, float camX, float camY) {
	int lastX = startX, lastY = startY;
	for (int i = 0; i < row_map.size(); i++){
		int tileID = row_map[i];
            if (tile_library.find(tileID) != tile_library.end() && tile_library.at(tileID) != nullptr) {
                Tile* currentTile = tile_library.at(tileID);
                lastX = currentTile->texture.x = lastX + (currentTile->offsetX * currentTile->scale);
                lastY = currentTile->texture.y = lastY + (currentTile->offsetY * currentTile->scale);                
                currentTile->texture.draw(renderer, camX, camY);
            }
	}
}

void ISODraw_Rectangle(float startX, float startY, SDL_Renderer* renderer, const std::vector<std::vector<int>>& rect_map, const std::unordered_map<int, Tile*>& tile_library, float camX, float camY) {
	int lastX = startX, lastY = startY;
    for (int i = 0; i < rect_map.size(); i++) {
		ISODraw_RowRight(lastX, lastY, renderer, rect_map[i], tile_library, camX, camY);
		if (tile_library.find(rect_map[i][0]) != tile_library.end() && tile_library.at(rect_map[i][0]) != nullptr) {
            Tile* firstTile = tile_library.at(rect_map[i][0]);
			lastX -= firstTile->offsetX * firstTile->scale;
			lastY += firstTile->offsetY * firstTile->scale;
        }

	}
}
