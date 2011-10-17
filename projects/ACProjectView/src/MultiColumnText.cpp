#include "MultiColumnText.h"
#include <spark/BaseApp.h>
#include <spark/SparkComponentFactory.h>
#include <spark/Window.h>
#include <animation/Easing.h>
#include <animation/AnimationManager.h>

using namespace spark;

namespace acprojectview {

    const char * const MultiColumnText::SPARK_TYPE = "MultiColumnText";
    
        
    MultiColumnText::MultiColumnText(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode),
          _myColumnSpace(_myXMLNode->getAttributeAs<int>("columnSpace", 10)),
          _myInitalXPos(_myXMLNode->getAttributeAs<int>("x", 0)),
          _myVisibleColumnIndex(0), _myColumnCount(0), _myAnimatingFlag(false) {


    }

    MultiColumnText::~MultiColumnText() {}

    void
    MultiColumnText::realize() {        
        Transform::realize();        
        _myTextPtrs.push_back(createNewColumn());
    }

    void 
    MultiColumnText::setMaxWidth(int theMaxWidth) {
        for (std::vector<spark::TextPtr>::size_type i = 0; i < _myTextPtrs.size(); i++) {
            _myTextPtrs[i]->setMaxWidth(theMaxWidth);
        }
    }
        
    void 
    MultiColumnText::setMaxHeight(int theMaxHeight) {
        for (std::vector<spark::TextPtr>::size_type i = 0; i < _myTextPtrs.size(); i++) {        
            _myTextPtrs[i]->setMaxHeight(theMaxHeight);
        }
    }
    
    void 
    MultiColumnText::onAnimationFinished() {
        _myAnimatingFlag = false;
    }
    
    void 
    MultiColumnText::swipe(int theDir) {
        int myNewColumnIndex = _myVisibleColumnIndex + theDir;
        if (_myColumnCount < 2 || 
            (_myVisibleColumnIndex == _myColumnCount-1 && theDir == 1 ) || 
            (_myVisibleColumnIndex == 0 && theDir == -1) || 
            _myAnimatingFlag) {
            // do nothing
            return;
        }  
        _myAnimatingFlag = true;
        MultiColumnTextPtr ptr = boost::static_pointer_cast<MultiColumnText>(shared_from_this());
        int myTargetX = -(_myTextPtrs[myNewColumnIndex]->getMaxWidth() + _myColumnSpace) * myNewColumnIndex;
        
        WidgetPropertyAnimationPtr changeAnimation0 = WidgetPropertyAnimationPtr(
                new WidgetPropertyAnimation(WidgetWeakPtr(WidgetPtr(ptr)), &Widget::setX, getX(), myTargetX, 300,
                    animation::EasingFnc(animation::easeInOutQuad)));        
        changeAnimation0->setOnFinish(masl::CallbackPtr(new MultiColumnTextCB(ptr, &MultiColumnText::onAnimationFinished)));
                            
        animation::AnimationManager::get().play(changeAnimation0);
        _myVisibleColumnIndex = myNewColumnIndex;
                        
    }
    void
    MultiColumnText::reset() {
        setX(_myInitalXPos);
        _myVisibleColumnIndex = 0;
    }
    
    void 
    MultiColumnText::setI18nId(const std::string & theNewI18nId) {
        setX(_myInitalXPos);
        _myVisibleColumnIndex = 0;
        _myColumnCount = 0;
        _myTextPtrs[0]->setI18nId(theNewI18nId);
        _myTextPtrs[0]->setX(_myColumnSpace);
        int myRenderedGlyphIndex = _myTextPtrs[0]->getRenderedGlyphIndex();
        while (myRenderedGlyphIndex > 0) {
            _myColumnCount++;
            if (_myColumnCount > _myTextPtrs.size()-1) {
                _myTextPtrs.push_back(createNewColumn());
            }
            TextPtr myColumn = _myTextPtrs[_myColumnCount];
            myColumn->setMaxWidth(_myTextPtrs[_myColumnCount-1]->getMaxWidth());            
            myColumn->setI18nId(theNewI18nId);
            myColumn->setStartIndex(_myTextPtrs[_myColumnCount-1]->getRenderedGlyphIndex());
            myColumn->setVisible(true);
            myColumn->setX(_myTextPtrs[_myColumnCount-1]->getX() + _myTextPtrs[_myColumnCount-1]->getMaxWidth() + _myColumnSpace);
            myColumn->setZ(_myTextPtrs[_myColumnCount-1]->getZ() + 1);
            myRenderedGlyphIndex = _myTextPtrs[_myColumnCount]->getRenderedGlyphIndex();
        }
        for (std::vector<spark::TextPtr>::size_type i = _myColumnCount+1; i < _myTextPtrs.size(); i++) {        
            _myTextPtrs[i]->setVisible(false);
        }
    }
    
    TextPtr 
    MultiColumnText::createNewColumn() {
        MultiColumnTextPtr ptr = boost::static_pointer_cast<MultiColumnText>(shared_from_this());
        BaseAppPtr myBaseAppPtr = boost::dynamic_pointer_cast<BaseApp>(getRoot());
        ComponentPtr myCreated = SparkComponentFactory::get().loadSparkComponentsFromString(myBaseAppPtr, 
                "<Text name=\"text_0\" maxHeight=\"250\" fontsize=\"13\" lineHeight=\"18\" font=\"/acswissmed.ttf\" sensible=\"false\" color=\"[1.0,1.0,1.0,1.0]\"/>"); 
        TextPtr myTextPtr = boost::static_pointer_cast<Text>(myCreated);        
        ContainerPtr myContainer = boost::static_pointer_cast<spark::Container>(ptr);
        myContainer->addChild(myTextPtr);
        return myTextPtr;
    }
     
}
