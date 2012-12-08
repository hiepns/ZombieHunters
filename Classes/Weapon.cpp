#include "Weapon.h"

Gun::Gun(int gunType) {

}

CCSprite* Gun::changeGun(int gunType) {

	return new CCSprite();
}

CCPoint Gun::getpos(int gunType) {
	return CCPointZero;
}

Axe::Axe(int axeType) {

}

CCSprite* Axe::changeAxe(int axeType) {
	return new CCSprite();
}

CCPoint Axe::getpos(int gunType) {
	return CCPointZero;
}
Bullet::Bullet(int _pos) {
	pos = _pos;
	exist = false;
	isRight = true;
}
void Bullet::onEnter(){
}
void Bullet::updatePos(float dt) {
	if (isRight) {

	}
}
