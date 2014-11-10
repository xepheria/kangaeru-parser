#include "HelloWorldScene.h"
#include "pugixml.hpp"
#include "kanjiEntry.h"

#include <iostream>
#include <string>

USING_NS_CC;



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    //now create the muthafuggin' Dictionary
    //finally.
    Dictionary *kanjiDict = Dictionary::create();
    
    //parse xml file into plist
    std::string source("../../../Resources/N5.xml");
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(source.c_str());
    if (!result)
    {
        std::cout << "XML [" << source << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
        std::cout << "Error description: " << result.description() << "\n";
    }
    
    pugi::xml_node entries = doc.child("data-set");
    std::cout << entries.name() << std::endl;
    
    std::string nameEntry;
    int i = 0;
    for(pugi::xml_node entry = entries.first_child(); entry; entry = entry.next_sibling())
    {
        //std::string kanjiName = entry.first_child().child_value();
        //std::cout << i << std::endl;
        Dictionary *in = Dictionary::create();
        //found an entry, print its attributes
        for(pugi::xml_node attr = entry.first_child(); attr; attr = attr.next_sibling())
        {
            //Now print all elements and attributes of current "Entry"
            std::string attrName = attr.name();
            std::string attrValstd = attr.child_value();
            String *attrVal = String::create(attrValstd);
            //std::cout << "name: " << attrName << " val: " << attrVal->getCString() << std::endl;
            in->setObject(attrVal, attrName);
        }
        kanjiDict->setObject(in, std::to_string(i));
        i++;
    }
    
    //iterate through the dictionary and print
    DictElement *pElement;
    CCDICT_FOREACH(kanjiDict, pElement)
    {
        const char *keyNum = pElement->getStrKey();
        const char *key;
        String *pStr;
        
        //std::cout << "---" << keyNum << "---" << std::endl;
        
        Dictionary *pDictInner = dynamic_cast<Dictionary*>(pElement->getObject());
        DictElement *pEleInner;
        CCDICT_FOREACH(pDictInner, pEleInner)
        {
            key = pEleInner->getStrKey();
            pStr = dynamic_cast<String*>(pEleInner->getObject());
            //std::cout << key << " " << pStr->getCString() << std::endl;
        }
    }
    
    if(!kanjiDict->writeToFile("../../../Resources/N5.plist"))
        std::cout << "Couldn't write to file." << std::endl;
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
