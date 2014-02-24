//
//  gameScene.h
//  Nyangame
//
//  Created by Mighty Craft Sapporo 02 on 2014/02/21.
//
//

#ifndef __Nyangame__gameScene__
#define __Nyangame__gameScene__

#include <iostream>
#include "cocos2d.h"
#include "config.h"

//  ブロック最大数
#define MAX_BLOCK_X 8
#define MAX_BLOCK_Y 8
//  消えるアニメーションの時間
#define REMOVING_TIME 0.1f
// 駒の移動時間
#define MOVING_TIME 0.2f

#define KEY_HIGHSCORE "HighScore"

//  背景
#define PNG_BACKGROUND "image/background.png"
#define PNG_GAMEOVER "image/gameover.png"
#define PNG_RESET "image/reset1.png"
#define MP3_REMOVE_BLOCK "sound/removeBlock.mp3"

#define FONT_RED "fonts/redFont.fnt"
#define FONT_BLUE "fonts/blueFont.fnt"
#define FONT_YELLOW "fonts/yellowFont.fnt"
#define FONT_GREEN "fonts/greenFont.fnt"
#define FONT_GRAY "fonts/grayFont.fnt"
#define FONT_WHITE "fonts/whiteFont.fnt"


class gameScene : public cocos2d::CCLayer{
protected:
    //  画像のタグ
    enum kTag{
        kTagBackground = 1,
        kTagRedLabel,
        kTagBlueLabel,
        kTagYellowLabel,
        kTagGreenLabel,
        kTagGrayLabel,
        kTagScoreLabel,
        kTagGameOver,
        kTagHighScoreLabel,
        kTagBaseBlock = 10000,
    };
    
    struct PositionIndex{
        PositionIndex(int x1, int y1){
            x = x1;
            y = y1;
        }
        
        int x;
        int y;
    };
    
    //  画像表示優先度
    enum kZOrder{
        kZOrderBackground,
        kZOrderLabel,
        kzOrderBlock,
        kZOrderGameOver,
    };
    
    cocos2d::CCSprite* m_background;
    //  駒のサイズ
    float m_blockSize;
    //  駒の種類ごとのグループ
    std::map<kBlock, std::list<int> > m_blockTags;
    
    std::vector<kBlock> blockTypes;
    
    //  アニメーションしてるかどうか
    bool m_animating;
    
    //  スコア
    int m_score;
    
protected:
    void showBackground();
    
    void initForVariables();
    void showBlock();
    
    cocos2d::CCPoint getPosition(int posIndexX, int posIndexY);
    int getTag(int posIndexX, int posIndexY);
    
    void getTouchBlockTag(cocos2d::CCPoint touchPoint, int &tag, kBlock &blockType);
    std::list<int> getSameColorBlockTags(int baseTag, kBlock blockType);
    void removeBlock(std::list<int> blockTags, kBlock blockType);
    bool hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag);
    
    void removingBlock(cocos2d::CCNode* block);
    
    PositionIndex getPositionIndex(int tag);
    void setNewPosition1(int tag, PositionIndex posIndex);
    void searchNewPosition1(std::list<int> blocks);
    void moveBlock();
    void movingBlocksAnimation1(std::list<int> blocks);
    
    void movedBlocks();
    std::map<int, bool> getExistBlockColumn();
    void searchNewPosition2();
    void setNewPosition2(int tag, PositionIndex posIndex);
    void movingBlocksAnimation2();
    
    void showLabel();
    
    bool existsSameBlock();
    
    void saveHighScore();
    void showHighScoreLabel();
    
    void menuResetCallback(cocos2d::CCObject* pSender);
    void showResetButton();
    
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(gameScene);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
};


#endif /* defined(__Nyangame__gameScene__) */
