//--------------------------------------------------------------------------------------------------------
// Dragon Drop - Marmalade SDK
//	Powered by Pocketeers Limited's IwGame Engine
//
// condition.h
//	Programmed by: Vanny Sou
//  Copyright 2018-2022
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
// Interface to track the condition of an actor
//--------------------------------------------------------------------------------------------------------

#if !defined(_CONDITION_H_)
#define _CONDITION_H_

enum e_conditions {CAN_ATTACK, CAN_HIT, CAN_REACH, FULL_LIFE, LOW_LIFE, INVULNERABLE, ENRAGED, ALIVE, STUNNED, CONSTRICTED, TOTAL_CONDITIONS};

class Condition
{
protected:
    unsigned int    m_type;
    bool            m_check;
    
public:
    bool            Check() const       { return m_check; }
    unsigned int    GetType() const     {return m_type; }
    void            SetBool(bool b)     {m_check = b;}
    
public:
    Condition(unsigned int type) : m_type(type), m_check(false)
    {
        // the following type need to be true
        if(m_type == FULL_LIFE || m_type == ALIVE)
            m_check = true;
    }
};

#endif
