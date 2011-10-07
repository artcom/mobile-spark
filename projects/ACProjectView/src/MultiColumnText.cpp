#include "MultiColumnText.h"
#include <spark/BaseApp.h>
#include <spark/SparkComponentFactory.h>
#include <spark/Window.h>

using namespace spark;

namespace acprojectview {

    const char * const MultiColumnText::SPARK_TYPE = "MultiColumnText";
    
        
    MultiColumnText::MultiColumnText(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : Transform(theApp, theXMLNode),
          _myColumnSpace(_myXMLNode->getAttributeAs<int>("columnSpace", 10)) {


    }

    MultiColumnText::~MultiColumnText() {}

    void
    MultiColumnText::realize() {        
        Transform::realize();        
        _myTextPtrs.push_back(createNewColumn());
    }

    void 
    MultiColumnText::setMaxWidth(int theMaxWidth) {
        for (unsigned int i = 0; i < _myTextPtrs.size(); i++) {
            _myTextPtrs[i]->setMaxWidth(theMaxWidth);
        }
    }
        
    void 
    MultiColumnText::setMaxHeight(int theMaxHeight) {
        for (unsigned int i = 0; i < _myTextPtrs.size(); i++) {        
            _myTextPtrs[i]->setMaxHeight(theMaxHeight);
        }
    }
    
    void 
    MultiColumnText::setI18nId(const std::string & theNewI18nId) {
        int myColumnCount = 0;
        _myTextPtrs[0]->setI18nId(theNewI18nId);
        _myTextPtrs[0]->setX(_myColumnSpace);
        unsigned myRenderedGlyphIndex = _myTextPtrs[0]->getRenderedGlyphIndex();
        unsigned myMessageLength = _myTextPtrs[0]->getTotalGlyphCount();
        while (myRenderedGlyphIndex < myMessageLength) {
            myColumnCount++;
            if (myColumnCount > _myTextPtrs.size()-1) {
                _myTextPtrs.push_back(createNewColumn());
            }
            TextPtr myColumn = _myTextPtrs[myColumnCount];
            myColumn->setMaxWidth(_myTextPtrs[myColumnCount-1]->getMaxWidth());            
            myColumn->setI18nId(theNewI18nId);
            myColumn->setStartIndex(_myTextPtrs[myColumnCount-1]->getRenderedGlyphIndex());
            myColumn->setVisible(true);
            myColumn->setX(_myTextPtrs[myColumnCount-1]->getX() + _myTextPtrs[myColumnCount-1]->getMaxWidth() + _myColumnSpace);
            myColumn->setZ(_myTextPtrs[myColumnCount-1]->getZ() + 1);
            myRenderedGlyphIndex = _myTextPtrs[myColumnCount]->getRenderedGlyphIndex();
        }
        for (int i = myColumnCount+1; i < _myTextPtrs.size(); i++) {        
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
