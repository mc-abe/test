//
//  BlockSprite.cpp
//  Nyangame
//
//  Created by Mighty Craft Sapporo 02 on 2014/02/21.
//
//

#include "BlockSprite.h"

BlockSprite::BlockSprite(){
    initNextPos();
}

BlockSprite::~BlockSprite(){
    
}

BlockSprite* BlockSprite::createWithBlockType(kBlock blockType){
    BlockSprite* pRet = new BlockSprite();
    if(pRet && pRet->initWithBlockType(blockType)){
        pRet->autorelease();
        return pRet;
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool BlockSprite::initWithBlockType(kBlock blockType){
    if(!CCSprite::initWithFile(getBlockImageFileName(blockType))){
        return false;
    }
    
    m_blockType = blockType;
    
    return true;
}

const char* BlockSprite::getBlockImageFileName(kBlock blockType){
    switch(blockType){
        case kBlockRed:
            return "image/red.png";
        case kBlockBlue:
            return "image/blue.png";
        case kBlockYellow:
            return "image/yellow.png";
        case kBlockGreen:
            return "image/green.png";
        case kBlockGray:
            return "image/gray.png";
        default:
            CCAssert(false, "invalid blockType");
            return "";
    }
}

//  移送先の初期化
void BlockSprite::initNextPos(){
    m_nextPosX = -1;
    m_nextPosY = -1;
}

//  移動先インデックスをセット
void BlockSprite::setNextPos(int nextPosX, int nextPosY){
    m_nextPosX = nextPosX;
    m_nextPosY = nextPosY;
}








