#include "Boss.h"
#include"Megaman.h"
#include"Boss_Bullet.h"
#include"Stage.h"

Boss* Boss::instance = 0;
Boss * Boss::getInstance()
{
	if (instance == 0)
		instance = new Boss();
	return instance;
}

void Boss::update()
{
	if (Stage::updating)
		return;

	updateVX();

	timeAttack.curLoop++;

	updateAnimation();

	updateBeforeHandle();
}

void Boss::updateAnimation()
{
	if (curAnimation == BA_STAND)
	{
		if (timeAction.curLoop > 100)
		{
			changeAction(BA_SHOOT);
			timeAction.start();
		}
		else
			timeAction.curLoop++;
	}
	else
		if (curAnimation == BA_SHOOT)
		{
			if (timeAction.curLoop>80)
			{
				changeAction(BA_PUSH);
				timeAction.start();
			}
			else
			{
				timeAction.curLoop++;
				if (curFrame == sprite->animates[curAnimation].nFrame - 1 && timeAttack.curLoop>25)
				{
					ENEMYBULLET->_Add(new Boss_Bullet(this));
					timeAttack.start();
				}
			}
		}

}

void Boss::updateVX()
{
	if (curAnimation == BA_STAND || curAnimation == BA_SHOOT)
		vx = 0;
	else
		if (curAnimation == BA_PUSH || curAnimation == BA_HIT)
			vx = BOSS_VX * direction;
}

void Boss::updateBeforeHandle()
{
	if (!alive)
		return;

	isOnGround = false;
	updateVelocity();
	BaseObject::update();

	if (delayAnimation.canCreateFrame())
	{
		if (curAnimation != nextAnimation)
		{
			curAnimation = nextAnimation;
			curFrame = 0;
		}
		else
			if (curFrame++ >= sprite->animates[curAnimation].nFrame - 1)
			{
				if (curAnimation ==BA_STAND )
					curFrame = (curFrame + 1) % sprite->animates[curAnimation].nFrame;
				else
					curFrame = sprite->animates[curAnimation].nFrame - 1;

			}
	}
}

void Boss::onCollision(BaseObject * other, int nx, int ny)
{
	Enemy::onCollision(other, nx, ny);

	if (other->collisionType == CT_GROUND && nx != 0)
	{
		direction = (Direction)nx;

		curAnimation = BA_STAND;
		nextAnimation = BA_STAND;
		curFrame = 0;
		timeAction.start();

	}
}

void Boss::restore(BaseObject * obj)
{
	Enemy::restore(obj);
	life = BOSS_LIFE;
	direction = Left;
}

Boss::Boss()
{
	sprite = SPRITEMANAGER->sprites[SPR_BOSS];
	damage = BOSS_DAMAGE;
	life = BOSS_LIFE;
	alive = true;
	width = 60;
	height = 74;

	timeAttack.init(0.1, 5);
	timeAttack.start();

	timeAction.init(10, 1000);
	timeAction.start();
}


Boss::~Boss()
{
}
