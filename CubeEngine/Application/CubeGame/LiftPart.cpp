#include "LiftPart.h"
#include "3D/Primitive/CubePrimitive.h"
#include "Scene/SceneMgr.h"
#include "Collision/PhysicsMgr.h"
#include "Island.h"
#include <algorithm>
#include "BuildingSystem.h"

namespace tzw
{
const float blockSize = 0.5;
LiftPart::LiftPart()
{
	m_liftHeight = 0.0;
	m_node = new CubePrimitive(blockSize, blockSize, blockSize);
	auto texture =  TextureMgr::shared()->getByPath("Texture/mud.jpg");
	m_node->getMaterial()->setTex("diffuseMap", texture);
	m_shape = new PhysicsShape();
	m_shape->initBoxShape(vec3(blockSize, blockSize, blockSize));
	m_parent = nullptr;
	m_effectedIslandGroup = "";
	for(int i = 0; i < 6; i++)
	{
		m_bearPart[i] = nullptr;
	}
	initAttachments();
}

Attachment * LiftPart::findProperAttachPoint(Ray ray, vec3 &attachPosition, vec3 &Normal, vec3 & attachUp)
{
	RayAABBSide side;
	vec3 hitPoint;
	auto isHit = ray.intersectAABB(m_node->localAABB(), &side, hitPoint);

	if (!isHit) return nullptr;


	auto m = m_node->getLocalTransform().data();
	vec3 up(m[4], m[5], m[6]);
	vec3 forward(-m[8], -m[9], -m[10]);
	vec3 right(m[0], m[1], m[2]);
	up.normalize();
	forward.normalize();
	right.normalize();
	Attachment * attachPtr = nullptr;
	switch (side)
	{
	case RayAABBSide::up:
	{
		attachPtr = getAttachmentInfo(4, attachPosition, Normal, attachUp);
	}
	break;
	case RayAABBSide::down:
	{
		attachPtr = getAttachmentInfo(5, attachPosition, Normal, attachUp);
	}
	break;
	case RayAABBSide::left:
	{
		attachPtr = getAttachmentInfo(3, attachPosition, Normal, attachUp);
	}
	break;
	case RayAABBSide::right:
	{
		attachPtr = getAttachmentInfo(2, attachPosition, Normal, attachUp);
	}
	break;
	case RayAABBSide::front:
	{
		attachPtr = getAttachmentInfo(0, attachPosition, Normal, attachUp);
	}
	break;
	case RayAABBSide::back:
	{
		attachPtr = getAttachmentInfo(1, attachPosition, Normal, attachUp);
	}
	break;
	default:
		break;
	}
	return attachPtr;
}

void LiftPart::initAttachments()
{
	//forward backward
	m_attachment[0] = new Attachment(vec3(0.0, 0.0, blockSize / 2.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 1.0, 0.0) ,this);
	m_attachment[1] = new Attachment(vec3(0.0, 0.0, -blockSize / 2.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0) ,this);

	//right left
	m_attachment[2] = new Attachment(vec3(blockSize / 2.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0) ,this);
	m_attachment[3] = new Attachment(vec3(-blockSize / 2.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0) ,this);

	//up down
	m_attachment[4] = new Attachment(vec3(0.0, blockSize / 2.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0) ,this);
	m_attachment[5] = new Attachment(vec3(0.0, -blockSize / 2.0, 0.0), vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, 1.0) ,this);
}

Attachment * LiftPart::getAttachmentInfo(int index, vec3 & pos, vec3 & N, vec3 & up)
{
	auto mat = m_node->getLocalTransform();
	auto atta = m_attachment[index];
	vec4 a_pos = vec4(atta->m_pos, 1.0);
	vec4 a_n = vec4(atta->m_normal, 0.0);
	vec4 a_up = vec4(atta->m_up, 0.0);
	pos = mat.transofrmVec4(a_pos).toVec3();
	N = mat.transofrmVec4(a_n).toVec3();
	up = mat.transofrmVec4(a_up).toVec3();
	return m_attachment[index];
}

Attachment* LiftPart::getFirstAttachment()
{
	return m_attachment[4];
}

void LiftPart::liftUp(float val)
{
	if (m_effectedIslandGroup.size()> 0) 
	{
		m_liftHeight += val;
		m_liftHeight = std::min(m_liftHeight, 10.0f);

		std::vector<Island *> groupList;
		BuildingSystem::shared()->getIslandsByGroup(m_effectedIslandGroup, groupList);
        for (auto island : groupList) 
		{
			auto oldPos = island->m_node->getPos();
			oldPos.y += val;
			island->m_node->setPos(oldPos);
        }
		vec3 oldPos = m_node->getPos();
		oldPos.y += val;
		m_node->setPos(oldPos);
	}
}

void LiftPart::setEffectedIsland(std::string islandGroup)
{
	m_effectedIslandGroup = islandGroup;
}

void LiftPart::cook()
{
	auto mat2 = m_node->getTranslationMatrix();
	auto aabb = m_node->getAABB();
	auto rigChasis = PhysicsMgr::shared()->createRigidBody(1.0, mat2, aabb);
	rigChasis->attach(m_node);
}

int LiftPart::getType()
{
	return GAME_PART_LIFT;
}
}