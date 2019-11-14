#include "3D/Primitive/CubePrimitive.h"
#include <algorithm>
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "Scene/SceneMgr.h"
#include "Collision/PhysicsCompoundShape.h"
#include "Collision/PhysicsMgr.h"
#include "CylinderPart.h"
#include "3D/Primitive/CylinderPrimitive.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "GameItem.h"

namespace tzw
{
	AttachmentInfo::AttachmentInfo(vec3 p, vec3 n, vec3 u):
	pos(p),normal(n),up(u)
	{
	}

	bool GameItem::isSpecialFunctionItem()
	{
		return (m_type == GamePartType::GAME_PART_LIFT) || (m_type == GamePartType::SPECIAL_PART_PAINTER);
	}

GameItem::GameItem()
{

}

	int GameItem::getTypeInInt()
	{
		return (int) m_type;
	}
}
