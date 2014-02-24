//
//  CCPlaySE.h
//  Nyangame
//
//  Created by Mighty Craft Sapporo 02 on 2014/02/21.
//
//

#ifndef __Nyangame__CCPlaySE__
#define __Nyangame__CCPlaySE__

#include <iostream>
#include "cocos2d.h"

class CCPlaySE : public cocos2d::CCActionInstant{
public:
    CCPlaySE(std::string sound);
    virtual ~CCPlaySE(){};
    
    virtual void update(float time);
    virtual CCFiniteTimeAction* reverse(void);
    virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);
    
public:
    static CCPlaySE* create(std::string sound);
    
protected:
    //  ファイル名
    std::string m_sound;
};

#endif /* defined(__Nyangame__CCPlaySE__) */
