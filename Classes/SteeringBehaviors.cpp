#include "SteeringBehaviors.h"
#include "GameCharacter.h"
#include "MathTool.h"
#include "EntityManager.h"

SteeringBehaviors::SteeringBehaviors( GameCharacter* owner ):m_arrivePrecision(1),
    m_separationMagnify(1500), m_wallAvoidanceMagnify(1000)
{
    m_pOwner        =   owner;
    m_behaviorsFlag =   NONE;
}

SteeringBehaviors::~SteeringBehaviors()
{

}

cocos2d::Vec2 SteeringBehaviors::calculate()
{
    m_vSteeringForce.setPoint(0, 0);

    /**
    * Ҫ֪��ÿһ�������������ȼ���ʵ�ǲ�ͬ�ģ�������Ҫ��Ҫ��֤����֮�䲻���ص���
    * ײǽ��Ȼ�����arrive��seek
    */
    if (On(WALL_AVOIDANCE) && !accumulateForce(m_vSteeringForce, wallAvoidance()))
    {
        return m_vSteeringForce;
    }
    if (On(SEPARATION) && !accumulateForce(m_vSteeringForce, separation()))
    {
        return m_vSteeringForce;
    }
    if (On(SEEK) && !accumulateForce(m_vSteeringForce, seek(m_vTarget)))
    {
        return m_vSteeringForce;
    }
    if (On(ARRIVE) && !accumulateForce(m_vSteeringForce, arrive(m_vTarget)))
    {
        return m_vSteeringForce;
    }

    return m_vSteeringForce;
}

cocos2d::Vec2 SteeringBehaviors::seek( const Vec2 &target )
{
    auto tmpMovingEntity    =   m_pOwner->getMovingEntity();
    Vec2 tmpDesiredVelocity = 
        (target - tmpMovingEntity.getPosition()).getNormalized()
        * tmpMovingEntity.getMaxSpeed();
    return tmpDesiredVelocity - tmpMovingEntity.getVelocity();
}

cocos2d::Vec2 SteeringBehaviors::arrive( const Vec2& target )
{
    auto tmpMovingEntity    =   m_pOwner->getMovingEntity();
    Vec2 tmpToTarget        =   target - tmpMovingEntity.getPosition();

    float dist  =   tmpToTarget.length();
    dist        =   dist < m_arrivePrecision ? 0 : dist;
    if (dist > 0)
    {
        // �������ٶȣ����������Ŀ϶��������ķ�ʽ�ӽ���ȥ
        float tmpSpeed  =   dist * 10;
        tmpSpeed = tmpSpeed > tmpMovingEntity.getMaxSpeed() ? tmpMovingEntity.getMaxSpeed() : tmpSpeed;
        Vec2 tmpDesiredVelocity =   tmpToTarget * tmpSpeed / dist;
        return tmpDesiredVelocity - tmpMovingEntity.getVelocity();
    }

    return Vec2(0,0);
}

cocos2d::Vec2 SteeringBehaviors::separation()
{
    Vec2 tmpForce;
    auto tmpIterator    =   EntityMgr->getEntityMap()->begin();
    for (; tmpIterator != EntityMgr->getEntityMap()->end(); tmpIterator++)
    {
        auto tmpCharacter   =   dynamic_cast<GameCharacter*>(tmpIterator->second);
        if (tmpCharacter == m_pOwner || !isNeighbor(tmpCharacter))
        {
            continue;
        }

        auto tmpToOwner =   m_pOwner->getMovingEntity().getPosition() - tmpCharacter->getMovingEntity().getPosition();
        // �����������ý�ɫ��owner�ľ���ɷ���
        auto tmpLen     =   tmpToOwner.getLength() - m_pOwner->getMovingEntity().getRadius();
        if (tmpLen <= 0)
        {
            tmpLen  =   1;
        }
        tmpForce        +=  m_separationMagnify * tmpToOwner.getNormalized() / tmpLen;
    }

    return tmpForce;
}

cocos2d::Vec2 SteeringBehaviors::wallAvoidance()
{
    auto tmpMovingEntity    =   m_pOwner->getMovingEntity();
    Vec2 tmpForce;
    auto tmpAllWalls    =   m_pOwner->getMapGrid()->getWalls();
    for (auto tmpIterator = tmpAllWalls.begin(); tmpIterator != tmpAllWalls.end(); tmpIterator++)
    {
        // �ֱ�������ÿ���ߵľ���
        auto tmpWallToOwner =   tmpMovingEntity.getPosition() - tmpIterator->getPointA();
        auto tmpDot         =   tmpWallToOwner.dot(tmpIterator->getNormal());
        if (tmpDot >= tmpMovingEntity.getRadius())
        {
            continue;
        }
        // @_@ ��������ȥ�ˣ�����ͻ����൱�ھ����൱��
        if (tmpDot <= 0)
        {
            tmpDot  =   1;
        }
        tmpForce    +=  m_wallAvoidanceMagnify * tmpIterator->getNormal() / tmpDot;
    }
    return tmpForce;
}

bool SteeringBehaviors::isNeighbor( GameCharacter* other )
{
    auto tmpOwnerMovingEntity   =   m_pOwner->getMovingEntity();
    auto tmpOtherMovingEntity   =   other->getMovingEntity();
    return (tmpOwnerMovingEntity.getPosition() - tmpOtherMovingEntity.getPosition()).getLengthSq() 
        <= tmpOwnerMovingEntity.getNeighborRadius() * tmpOwnerMovingEntity.getNeighborRadius();
}

bool SteeringBehaviors::accumulateForce( Vec2& steeringForce, Vec2 force )
{
    auto tmpOwnerMovingEntity   =   m_pOwner->getMovingEntity();
    auto tmpLeftForce           =   tmpOwnerMovingEntity.getMaxForce() - steeringForce.getLength();
    
    // �����ǰ���������Ѿ��������������������
    if (tmpLeftForce <= 0)
    {
        return false;
    }

    if (tmpLeftForce >= force.getLength())
    {
        steeringForce   +=  force;
    }
    else
    {
        steeringForce   +=  force.getNormalized() * tmpLeftForce;
    }
    return true;
}