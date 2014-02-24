//
//  BlockSprite.h
//  Nyangame
//
//  Created by Mighty Craft Sapporo 02 on 2014/02/21.
//
//

#ifndef __Nyangame__BlockSprite__
#define __Nyangame__BlockSprite__

#include <iostream>
#include "cocos2d.h"
#include "config.h"

class BlockSprite : public cocos2d::CCSprite{
    
protected:
    const char* getBlockImageFileName(kBlock blockType);
public:
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(kBlock, m_blockType, BlockType);
    CC_SYNTHESIZE_READONLY(int, m_nextPosX, NextPosX);
    CC_SYNTHESIZE_READONLY(int, m_nextPosY, NextPosY);
    
    void initNextPos();
    void setNextPos(int nextPosX, int nextPosY);
    
    BlockSprite();
    virtual ~BlockSprite();
    
    virtual bool initWithBlockType(kBlock blockType);
    static BlockSprite* createWithBlockType(kBlock blockType);
};

#endif /* defined(__Nyangame__BlockSprite__) */
