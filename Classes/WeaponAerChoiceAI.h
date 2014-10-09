#ifndef __WEAPON_AER_CHOICE_AI_H__
#define __WEAPON_AER_CHOICE_AI_H__

#include "WeaponChoiceAI.h"

/**
*	 骑士选择武器的AI
*/
class WeaponAerChoiceAI : public WeaponChoiceAI
{
public:
    WeaponAerChoiceAI(GameCharacter* owner):WeaponChoiceAI(owner), m_cryosphereProbability(0.1)
    {

    }

    virtual ~WeaponAerChoiceAI()
    {

    }

protected:
    /**
    *	 骑士只有两种攻击方式，普通的近距离攻击和冰圈攻击，都是近距离攻击，当前的逻辑
    *    就是有概率的使用冰圈攻击
    */
    virtual void choiceWeapon() override
    {
        changeWeapon(choiceCloseOrCryosphere());
    }

private:
    WeaponTypeEnum choiceCloseOrCryosphere()
    {
        // 暂时骑士只有普通攻击
        return NORMAL_CLOSE_RANGE_WEAPON;

        if (CCRANDOM_0_1() < m_cryosphereProbability)
        {
            return AER_CRYOSPHERE_SKILL_WEAPON;
        }
        else
        {
            return NORMAL_CLOSE_RANGE_WEAPON;
        }
    }

    const float m_cryosphereProbability;            // 冰圈的概率
};

#endif