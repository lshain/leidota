#ifndef __GOAL_H__
#define __GOAL_H__

template <class entity_type>
class GoalComposite;

/**
*  ����Ŀ��Ļ��࣬�����ԭ��Ŀ�꣬���ܴ�����Ŀ�꣬��Ŀ��Ӧ��������ȥ�ص�ʵ��Ľӿ�
*  ������ʵ����ɸ�Ŀ��
*/
template <class entity_type>
class Goal
{
protected:
    /**
    * ��ǰĿ���״̬��ö��
    */
    enum GoalStateEnum
    {
        active,
        inactive,
        completed,
        failed
    };

public:

    /**
    * �Ե�ǰ״̬���ж� 
    */
    bool isComplete()const{return m_goalState == completed;} 
    bool isActive()const{return m_goalState == active;}
    bool isInactive()const{return m_goalState == inactive;}
    bool hasFailed()const{return m_goalState == failed;}

protected:
    Goal(entity_type* owner)
    {
        m_pOwner    =   owner;
        m_goalState =   inactive;
    }

    virtual ~Goal()
    {

    }

    /**
    * Ϊ�˷������ֻ�д���δ����״̬��ʱ��ŵ���activate��
    */
    void activateIfInactive()
    {
        if (isInactive())
        {
            m_goalState = active;
            activate();
        }
    }

    /**
    *  �����Ŀ����߼���Ҳ���ǿ�ʼ���ո�Ŀ���Ҫȥ������ɫʵ�ָ�Ŀ��
    */
    virtual void activate() = 0;

    /**
    * ��ʱ������Ŀ�꣬����᷵�ظ�Ŀ��ĵ�ǰ״̬�������Ŀ�� 
    */
    virtual GoalStateEnum process() = 0;

    /**
    * ��Ŀ�������ʱ�����õģ�����ȡ����ǰĿ���
    */
    virtual void terminate() {m_goalState = completed;}

    /**
    * ԭ��Ŀ�����޷�����Ŀ��� 
    */
    virtual void addSubgoal(Goal<entity_type>* goal)
    {
        throw std::runtime_error("Cannot add goals to atomic goals");
    }

    friend class GoalComposite<entity_type>;

protected:
    GoalStateEnum       m_goalState;                    // Ŀ��״̬
    entity_type*        m_pOwner;                       // Ŀ���ӵ����
};

#endif