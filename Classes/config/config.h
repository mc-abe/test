//
//  config.h
//  Nyangame
//
//  Created by Mighty Craft Sapporo 02 on 2014/02/21.
//
//

#ifndef Nyangame_config_h
#define Nyangame_config_h

enum kBlock{
    kBlockRed,
    kBlockBlue,
    kBlockYellow,
    kBlockGreen,
    kBlockGray,
    kBlockCount,
};

#define ccsf(...) CCString::createWithFormat(__VA_ARGS__)->getCString()

#endif
