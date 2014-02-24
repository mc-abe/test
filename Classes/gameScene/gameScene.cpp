//
//  gameScene.cpp
//  Nyangame
//
//  Created by Mighty Craft Sapporo 02 on 2014/02/21.
//
//

#include "gameScene.h"
#include "SimpleAudioEngine.h"
#include "BlockSprite.h"
#include "CCPlaySE.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

CCScene* gameScene::scene(){
    CCScene* scene = CCScene::create();
    gameScene* layer = gameScene::create();
    scene->addChild(layer);
    return scene;
}

//  初期化
bool gameScene::init(){
    if(!CCLayer::init()){
        return false;
    }
    
    //  タッチイベントの取得
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    
    //  バックキー・メニューキー設定追加
    setKeypadEnabled(true);
    
    //  変数初期化
    this->initForVariables();
    
    //  背景表示
    this->showBackground();
    
    //  駒表示
    this->showBlock();
    
    showLabel();
    showHighScoreLabel();
    
    showResetButton();
    
    //  SEのロード
    SimpleAudioEngine::sharedEngine()->preloadEffect(MP3_REMOVE_BLOCK);
    
    return true;
}

bool gameScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    //  アニメーション中はタッチ処理を受け付けない
    return !m_animating;
}

void gameScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    //  タッチポイント取得
    CCPoint touchPoint = m_background->convertTouchToNodeSpace(pTouch);
    
    //  タップした駒のタグと駒の種類を取得
    int tag = 0;
    kBlock blockType;
    getTouchBlockTag(touchPoint, tag, blockType);
    
    if(tag != 0){
        //  隣接する駒を検索
        list<int> sameColorBlockTags = getSameColorBlockTags(tag, blockType);
        
        if(sameColorBlockTags.size() > 1){
            //  特典加算
            m_score += pow(sameColorBlockTags.size() - 2, 2);
            //  アニメーション開始
            m_animating = true;
            //  隣接する駒を削除
            removeBlock(sameColorBlockTags, blockType);
            
            //  駒削除後のアニメーション
            movingBlocksAnimation1(sameColorBlockTags);
        }
    }
    showLabel();
}

//  バックキー処理
void gameScene::keyBackClicked(){
    CCDirector::sharedDirector()->end();
}

//  メニューキー処理
void gameScene::keyMenuClicked(){
    menuResetCallback(NULL);
}

//  背景表示
void gameScene::showBackground(){
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    
    //  背景の生成
    this->m_background = CCSprite::create(PNG_BACKGROUND);
    this->m_background->setPosition(ccp(winsize.width / 2, winsize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
}

//  変数初期化
void gameScene::initForVariables(){
    //  乱数初期化
    srand((unsigned)time(NULL));
    
    //  駒のいっぺんの長さを取得
    BlockSprite* pBlock = BlockSprite::createWithBlockType(kBlockRed);
    this->m_blockSize = pBlock->getContentSize().height;
    
    //  駒の種類の生成配列
    blockTypes.push_back(kBlockRed);
    blockTypes.push_back(kBlockBlue);
    blockTypes.push_back(kBlockYellow);
    blockTypes.push_back(kBlockGreen);
    blockTypes.push_back(kBlockGray);
    
    m_animating = false;
    m_score = 0;
}

//  位置取得
CCPoint gameScene::getPosition(int posIndexX, int posIndexY){
    float offsetX = m_background->getContentSize().width * 0.168f;
    float offsetY = m_background->getContentSize().height * 0.029f;
    
    return CCPoint((posIndexX + 0.5f) * m_blockSize + offsetX,
                   (posIndexY + 0.5f) * m_blockSize + offsetY);
}

//  タグ取得
int gameScene::getTag(int posIndexX, int posIndexY){
    return kTagBaseBlock + posIndexX * 100 + posIndexY;
}

//  駒表示
void gameScene::showBlock(){
    //  8x8の駒を生成
    for(int x = 0; x < MAX_BLOCK_X; x++){
        for(int y = 0; y < MAX_BLOCK_Y; y++){
            //  ランダムで駒作成
            kBlock blockType = (kBlock)(rand() % kBlockCount);
            
            //  対応する駒配列にタグを追加
            int tag = getTag(x, y);
            m_blockTags[blockType].push_back(tag);
            
            //  駒を作成
            BlockSprite* pBlock = BlockSprite::createWithBlockType(blockType);
            pBlock->setPosition(getPosition(x, y));
            m_background->addChild(pBlock, kzOrderBlock,tag);
        }
    }
}

//  タップされた駒のタグを取得
void gameScene::getTouchBlockTag(cocos2d::CCPoint touchPoint, int &tag, kBlock &blockType){
    for(int x = 0; x < MAX_BLOCK_X; x++){
        for(int y = 0; y < MAX_BLOCK_Y; y++){
            int currentTag = getTag(x, y);
            //  駒を取得
            CCNode* node = this->m_background->getChildByTag(currentTag);
            if(node && node->boundingBox().containsPoint(touchPoint)){
                tag = currentTag;
                blockType = ((BlockSprite*)node)->getBlockType();
            }
        }
    }
}

//  駒配列にあるか検索
bool gameScene::hasSameColorBlock(std::list<int> blockTagList, int searchBlockTag){
    list<int>::iterator it;
    for(it = blockTagList.begin(); it != blockTagList.end(); ++it){
        if(*it == searchBlockTag){
            return true;
        }
    }
    return false;
}

//  タップされた駒と同色かつ接している駒の配列を返す
list<int> gameScene::getSameColorBlockTags(int baseTag, kBlock blockType){
    //  同色の駒を格納する配列の初期化
    list<int> sameColorBlockTags;
    sameColorBlockTags.push_back(baseTag);
    
    list<int>::iterator it = sameColorBlockTags.begin();
    
    while(it != sameColorBlockTags.end()){
        int tags[] =
        {
            *it + 100,  //  右ブロックのタグ
            *it - 100,  //  左ブロックのタグ
            *it + 1,    //  上ブロックのタグ
            *it - 1,    //  下ブロックのタグ
        };
        
        for(int i = 0; i < sizeof(tags) / sizeof(tags[0]); i++){
            //  既にリストにあるか検索
            if(!hasSameColorBlock(sameColorBlockTags, tags[i])){
                //  駒配列にあるか検索
                if(hasSameColorBlock(m_blockTags[blockType], tags[i])){
                    sameColorBlockTags.push_back(tags[i]);
                }
            }
        }
        it++;
    }
    return sameColorBlockTags;
}

//  駒の削除
void gameScene::removingBlock(cocos2d::CCNode *block){
    block->removeFromParentAndCleanup(true);
}

gameScene::PositionIndex gameScene::getPositionIndex(int tag){
    int pos1_x = (tag - kTagBaseBlock) / 100;
    int pos1_y = (tag - kTagBaseBlock) % 100;
    
    return PositionIndex(pos1_x, pos1_y);
}

//  駒を消去
void gameScene::removeBlock(list<int> blockTags, kBlock blockType){
    
    bool first = true;
    
    list<int>::iterator it = blockTags.begin();
    while(it != blockTags.end()){
        //  既存配列から該当駒を削除
        m_blockTags[blockType].remove(*it);
        
        //  対象となる駒を検索
        CCNode* block = m_background->getChildByTag(*it);
        if(block){
            //駒が消えるアニメーションを生成
            CCScaleTo* scale = CCScaleTo::create(REMOVING_TIME, 0.0f);
            
            //  駒を削除するアクションを生成
            CCCallFuncN* func = CCCallFuncN::create(this, callfuncN_selector(gameScene::removingBlock));
            
            //  アクションをつなげる
            CCFiniteTimeAction* sequence = CCSequence::create(scale, func, NULL);
            
            CCFiniteTimeAction* action;
            if(first){
                //  駒が消えるサウンドアクションを生成
                CCPlaySE* playSe = CCPlaySE::create(MP3_REMOVE_BLOCK);
                //  アクションをつなげる
                action = CCSpawn::create(sequence, playSe, NULL);
                
                first = false;
            }
            else{
                action = sequence;
            }
            
            //  アクションをセット
            block->runAction(action);
        }
        
        it++;
    }
    
    SimpleAudioEngine::sharedEngine()->playEffect(MP3_REMOVE_BLOCK);
}

//  新規の座標をセット
void gameScene::setNewPosition1(int tag, gameScene::PositionIndex posIndex){
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosY = blockSprite->getNextPosY();
    if(nextPosY == -1){
        nextPosY = posIndex.y;
    }
    
    //  移送先の位置をセット
    blockSprite->setNextPos(posIndex.x, --nextPosY);
}

//  消えた駒の位置に駒をセット
void gameScene::searchNewPosition1(std::list<int> blocks){
    //  消える駒の分ループ
    list<int>::iterator it1 = blocks.begin();
    while(it1 != blocks.end()){
        PositionIndex posIndex1 = getPositionIndex(*it1);
        
        //  駒の種類のループ
        vector<kBlock>::iterator it2 = blockTypes.begin();
        while(it2 != blockTypes.end()){
            //  各種駒分ループ
            list<int>::iterator it3 = m_blockTags[*it2].begin();
            while(it3 != m_blockTags[*it2].end()){
                PositionIndex posIndex2 = getPositionIndex(*it3);
                
                if(posIndex1.x == posIndex2.x && posIndex1.y < posIndex2.y){
                    //  移動先の位置をセット
                    setNewPosition1(*it3, posIndex2);
                }
                it3++;
            }
            it2++;
        }
        it1++;
    }
}

//  駒の移動
void gameScene::moveBlock(){
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end()) {
            BlockSprite* blockSptrite = (BlockSprite*)m_background->getChildByTag(*it2);
            
            int nextPosX = blockSptrite->getNextPosX();
            int nextPosY = blockSptrite->getNextPosY();
            
            if(nextPosX != -1 || nextPosY != -1){
                //  新しいタグをセット
                int newTag = getTag(nextPosX, nextPosY);
                blockSptrite->initNextPos();
                blockSptrite->setTag(newTag);
                
                //  新しいタグに変更
                *it2 = newTag;
                
                //  アニメーションのセット
                CCMoveTo* move = CCMoveTo::create(MOVING_TIME, getPosition(nextPosX, nextPosY));
                blockSptrite->runAction(move);
            }
            it2++;
        }
        it1++;
    }
}

//  駒消去後のアニメーション
void gameScene::movingBlocksAnimation1(std::list<int> blocks){
    //  駒の新しい位置をセット
    searchNewPosition1(blocks);
    
    //  セットされた駒のアニメーション
    moveBlock();
    
    scheduleOnce(schedule_selector(gameScene::movingBlocksAnimation2), MOVING_TIME);
}

//  駒の移動が終了
void gameScene::movedBlocks(){
    
    m_animating = false;
    
    //  ゲーム終了チェック
    if(!existsSameBlock()){
        //  ハイスコアを記録
        saveHighScore();
        
        CCSize bgSize = m_background->getContentSize();
        
        //  ゲーム終了表示
        CCSprite* gameOver = CCSprite::create(PNG_GAMEOVER);
        gameOver->setPosition(ccp(bgSize.width / 2, bgSize.height * 0.8f));
        m_background->addChild(gameOver, kZOrderGameOver, kTagGameOver);
        
        setTouchEnabled(false);
    }
}

//  新しい位置をセット
void gameScene::setNewPosition2(int tag, gameScene::PositionIndex posIndex){
    BlockSprite* blockSprite = (BlockSprite*)m_background->getChildByTag(tag);
    int nextPosX = blockSprite->getNextPosX();
    if(nextPosX == -1){
        nextPosX = posIndex.x;
    }
    
    //  移動先の座標をセット
    blockSprite->setNextPos(--nextPosX, posIndex.y);
}

//  存在する列を取得
map<int, bool> gameScene::getExistBlockColumn(){
    //  検索配列初期化
    map<int, bool> xBlock;
    for(int i = 0; i < MAX_BLOCK_X; i++){
        xBlock[i] = false;
    }
    
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end()) {
            xBlock[getPositionIndex(*it2).x] = true;
            
            it2++;
        }
        
        it1++;
    }
    
    return xBlock;
}

//  新しい位置をセット
void gameScene::searchNewPosition2(){
    //  存在する列を取得
    map<int, bool> xBlock = getExistBlockColumn();
    
    bool first = true;
    for(int i = MAX_BLOCK_X - 1; i >= 0; i--){
        if(xBlock[i]){
            //  駒が存在
            first = false;
            continue;
        }
        else{
            //  駒が存在しない
            if(first){
                continue;
            }
            else{
                vector<kBlock>::iterator it1 = blockTypes.begin();
                while(it1 != blockTypes.end()){
                    list<int>::iterator it2 = m_blockTags[*it1].begin();
                    while(it2 != m_blockTags[*it1].end()){
                        PositionIndex posIndex = getPositionIndex(*it2);
                        
                        if(i < posIndex.x){
                            setNewPosition2(*it2, posIndex);
                        }
                        it2++;
                    }
                    it1++;
                }
            }
        }
    }
}

//  駒のアニメーション
void gameScene::movingBlocksAnimation2(){
    searchNewPosition2();
    
    moveBlock();
    
    scheduleOnce(schedule_selector(gameScene::movedBlocks), MOVING_TIME);
}

//  駒の数を表示
void gameScene::showLabel(){
    CCSize bgSize = m_background->getContentSize();
    
    //  残数表示
    int tagsForLabel[] = {kTagRedLabel, kTagBlueLabel, kTagYellowLabel,
                            kTagGreenLabel, kTagGrayLabel};
    const char* fontNames[] = {FONT_RED, FONT_BLUE, FONT_YELLOW, FONT_GREEN, FONT_GRAY};
    float heightRate[] = {0.61f, 0.51f, 0.41f, 0.31f, 0.21f};
    
    vector<kBlock>::iterator it = blockTypes.begin();
    while(it != blockTypes.end()){
        //  駒残数表示
        int count = m_blockTags[*it].size();
        const char* countStr = ccsf("%02d", count);
        CCLabelBMFont* label = (CCLabelBMFont*)m_background->getChildByTag(tagsForLabel[*it]);
        if(!label){
            //  駒残数生成
            label = CCLabelBMFont::create(countStr, fontNames[*it]);
            label->setPosition(ccp(bgSize.width * 0.78f, bgSize.height * heightRate[*it]));
            m_background->addChild(label, kZOrderLabel, tagsForLabel[*it]);
        }
        else{
            label->setString(countStr);
        }
        it++;
    }
    
    //  スコア表示
    const char* scoreStr = ccsf("%d", m_score);
    CCLabelBMFont* scoreLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagScoreLabel);
    if(!scoreLabel){
        scoreLabel = CCLabelBMFont::create(scoreStr, FONT_WHITE);
        scoreLabel->setPosition(ccp(bgSize.width * 0.78f, bgSize.height * 0.75f));
        m_background->addChild(scoreLabel, kZOrderLabel, kTagScoreLabel);
    }
    else{
        scoreLabel->setString(scoreStr);
    }
}

//  隣り合う駒があるかをチェック
bool gameScene::existsSameBlock(){
    vector<kBlock>::iterator it1 = blockTypes.begin();
    while (it1 != blockTypes.end()) {
        list<int>::iterator it2 = m_blockTags[*it1].begin();
        while (it2 != m_blockTags[*it1].end()) {
            if(getSameColorBlockTags(*it2, *it1).size() > 1){
                return true;
            }
            it2++;
        }
        it1++;
    }
    return false;
}

//  ハイスコアの表示
void gameScene::showHighScoreLabel(){
    CCSize bgSize = m_background->getContentSize();
    
    //  ハイスコア表示
    int highScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_HIGHSCORE, 0);
    const char* highScoreStr = ccsf("%d", highScore);
    CCLabelBMFont* highScoreLabel = (CCLabelBMFont*)m_background->getChildByTag(kTagHighScoreLabel);
    if(!highScoreLabel){
        //  ハイスコア生成
        highScoreLabel = CCLabelBMFont::create(highScoreStr, FONT_WHITE);
        highScoreLabel->setPosition(ccp(bgSize.width * 0.78f, bgSize.height * 0.87f));
        m_background->addChild(highScoreLabel, kZOrderLabel, kTagHighScoreLabel);
    }
}

//  ハイスコアの記録
void gameScene::saveHighScore(){
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    
    //  ハイスコアを取得
    int oldHighScore = userDefault->getIntegerForKey(KEY_HIGHSCORE, 0);
    if(oldHighScore < m_score){
        //  ハイスコアを保持
        userDefault->setIntegerForKey(KEY_HIGHSCORE, m_score);
        userDefault->flush();
        
        //  ハイスコアを表示
        showHighScoreLabel();
    }
}

//  リセットボタンタッチ時の処理
void gameScene::menuResetCallback(cocos2d::CCObject *pSender){
    gameScene* scene = gameScene::create();
    CCDirector::sharedDirector()->replaceScene((CCScene*)scene);
}

void gameScene::showResetButton(){
    CCSize bgSize = m_background->getContentSize();
    
    //  リセットボタン作成
    CCMenuItemImage* resetButton = CCMenuItemImage::create(PNG_RESET, PNG_RESET,
                                                           this, menu_selector(gameScene::menuResetCallback));
    resetButton->setPosition(ccp(bgSize.width * 0.78f, bgSize.height * 0.1f));
    
    //  メニュー作成
    CCMenu* menu = CCMenu::create(resetButton, NULL);
    menu->setPosition(CCPointZero);
    
    m_background->addChild(menu);
    
}






