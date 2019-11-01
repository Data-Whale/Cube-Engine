#include "SpinNode.h"
#include "CubeGame/ResNode.h"
#include "../BearPart.h"

namespace tzw
{
	SpinNode::SpinNode()
	{
		name =u8"��ת";
		m_bearingAttr = addIn(u8"���");
		m_signalAttr = addIn(u8"�����ź�");
	}

	NodeAttrValue SpinNode::execute()
	{
		auto attrVal = m_bearingAttr->eval();
		for(auto val : attrVal.m_list)
		{
			auto node = static_cast<ResNode *>(val.usrPtr);
			auto constraint = dynamic_cast<BearPart *>(node->getProxy());

			int signal = m_signalAttr->eval().getInt();
			if(constraint)
			{
				if(constraint->getIsSteering())
				{
					if(signal!= 0)
					{
						constraint->enableAngularMotor(true, 1.0f * signal, 50);
					}else
					{
						constraint->enableAngularMotor(true, 0, 10000000.0f);
					}
				}else
				{
					if(signal!= 0)
					{
						constraint->enableAngularMotor(true, 10.0f * signal, 50);
					}else
					{
						constraint->enableAngularMotor(false, 10.0f, 50);
					}
				}
			}
		}
		return NodeAttrValue();
	}

	int SpinNode::getNodeClass()
	{
		return Node_CLASS_SPIN;
	}
}