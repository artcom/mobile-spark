#include "I18nContext.h"

namespace spark {

    ///////////////////////////////////////////////////////I18nContext
    const char * const I18nContext::SPARK_TYPE = "I18nContext";

    I18nContext::I18nContext(const spark::BaseAppPtr& theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
        Container(theApp, theXMLNode, theParent) {
        defaultLanguage_ = _myXMLNode->getAttributeAs<std::string>("defaultLanguage","en"); 
        AC_PRINT << "default language " << defaultLanguage_;
    }

    I18nContext::~I18nContext() {
    }

    void
    I18nContext::switchLanguage(const std::string & theLanguage) {
        AC_INFO << "I18n context " << getName() << " switching to language " << theLanguage << " _myLanguage: " << language_;
        if (language_ != theLanguage) {
            language_ = theLanguage;

            for (std::vector<ComponentPtr>::iterator it = _myChildren.begin(); it != _myChildren.end(); ++it) {
                I18nItemPtr myItem = boost::static_pointer_cast<I18nItem>(*it);
                if (myItem) myItem->switchLanguage(theLanguage);
            }
        }
        EventPtr myEvent = EventFactory::get().handleEvent("<I18nEvent type=\"on_language_switch\">");
        (*myEvent)();
    }

    void
    I18nContext::addChild(ComponentPtr theChild) {
        I18nItemPtr myChild = boost::static_pointer_cast<I18nItem>(theChild);
        if (!myChild) {
            throw I18nItemNotFoundException("adding child " + theChild->getName() + " to I18nContext " + getName() + " does not work because theChild is not an I18nItem", PLUS_FILE_LINE);
        }
        Container::addChild(theChild);
        if (language_ != "") {
            myChild->switchLanguage(language_);
        }
    }




    ///////////////////////////////////////////////////////I18nItem
    I18nItem::I18nItem(const spark::BaseAppPtr & theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
              Component(theXMLNode, theParent){
    }
    I18nItem::~I18nItem() {
    }
    void 
    I18nItem::switchLanguage(const std::string & theLanguage) {
        AC_PRINT << "item switch language";
    }



    ///////////////////////////////////////////////////////I18nText
    const char * const I18nText::SPARK_TYPE = "I18nText";
    I18nText::I18nText(const spark::BaseAppPtr & theApp, const XMLNodePtr theXMLNode, ComponentPtr theParent):
              I18nItem(theApp, theXMLNode, theParent) {
    }

    I18nText::~I18nText() {
    }

    std::string &  
    I18nText::getText() {
        return text_;
    }

}

/*

spark.I18nContext.Constructor = function (Protected) {
    Public.Inherit(spark.EventDispatcher);


    Base.postRealize = Public.postRealize;
    Public.postRealize = function () {
        Base.postRealize();
        _myDefaultLanguage = Protected.getString("defaultLanguage", "en");
        Public.switchLanguage(_myDefaultLanguage);
    };

    Base.addChild = Public.addChild;
    Public.addChild = function (theChild) {
        Base.addChild(theChild);
        if (_myLanguage !== "") {
            theChild.switchLanguage(_myLanguage);
        }
    };
};
*/


/*
spark.I18nItem = spark.AbstractClass("I18nItem");

spark.I18nItem.Constructor = function (Protected) {
    var Public = this;
    var Base = {};

    Public.Inherit(spark.Component);
    Public.Inherit(spark.EventDispatcher);

    var _myLanguage = "";

    Public.__defineGetter__("language", function () {
        return _myLanguage;
    });

    var _myLanguageData = {};
    var _myLanguageNodes = {}; 

    Protected.languageData = function () {
        return _myLanguageData;
    };

    Base.realize = Public.realize;
    Public.realize = function () {
        Base.realize();
        var myNode = Public.node;
        for (var i = 0; i < myNode.childNodesLength(); i++) {
            var myChild = myNode.childNode(i);
            var myLang = myChild.nodeName;
            var myData = "";
            var myNumChildren = myChild.childNodesLength();
            for (var j = 0; j < myNumChildren; j++) {
                myData += myChild.childNode(j).nodeValue;
            }
            Public.addLanguageData(myLang, myData);
            Public.addLanguageNode(myLang, myChild);
        }
    };

    Protected.createEvent = function (theLanguage) {
        return new spark.I18nEvent(theLanguage);
    };

    Public.getLanguageData = function (theLanguage) {
        if (!theLanguage) {
            theLanguage = _myLanguage;
        }
        if (!(theLanguage in _myLanguageData)) {
            Logger.debug("I18n item '" + Public.name + "' does not contain language '" + theLanguage + "'");
            if (Public.parent.defaultLanguage in _myLanguageData) {
                return _myLanguageData[Public.parent.defaultLanguage];
            } else {
                Logger.warning("I18n item '" + Public.name + "' does not contain defaultlanguage '" + Public.parent.defaultLanguage + "'");
                return null;
            }
        } else {
            return _myLanguageData[theLanguage];
        }
    };

    Public.hasLanguageData = function (theLanguage) {
        return theLanguage in _myLanguageData;
    };
    
    Public.getLanguageNode = function(theLanguage) {
        if(!theLanguage) {
            theLanguage = _myLanguage;
        }
        if(!(theLanguage in _myLanguageNodes)) {
            Logger.info("I18n item '" + Public.name + "' does not contain language '" + theLanguage + "'");
            if (Public.parent.defaultLanguage in _myLanguageNodes) {
                return _myLanguageNodes[Public.parent.defaultLanguage];
            } else {
                Logger.warning("I18n item '" + Public.name + "' does not contain defaultlanguage '" + Public.parent.defaultLanguage + "'");
                return null;
            }
        } else {
            return _myLanguageNodes[theLanguage];
        }
    };
    
    Public.hasLanguageNode = function(theLanguage) {
        return theLanguage in _myLanguageNodes;
    };

    Public.switchLanguage = function (theLanguage) {
        if (theLanguage == _myLanguage) {
            return;
        }

        // XXX; one of these tests is redundant now
        if (!(theLanguage in _myLanguageData)) {
            Logger.debug("I18n item " + Public.name + " does not contain language " + theLanguage);
        }
        if(!(theLanguage in _myLanguageNodes)) {
            Logger.debug("I18n item " + Public.name + " does not contain language node " + theLanguage);
        }
        _myLanguage = theLanguage;
        var myEvent = Protected.createEvent(theLanguage);
        Public.dispatchEvent(myEvent);
    };

    Public.addLanguageData = function (theLanguage, theData) {
        if (theLanguage in _myLanguageData) {
            Logger.warning("duplicate i18n data for item " + Public.name + " in language " + theLanguage);
        }
        _myLanguageData[theLanguage] = theData;
    };
    	   
    Public.addLanguageNode = function(theLanguage, theNode) {
        if(theLanguage in _myLanguageNodes) {
            Logger.warning("duplicate i18n node for item " + Public.name + " in language " + theLanguage);
        }
        _myLanguageNodes[theLanguage] = theNode;
    };

    Public.changeLanguageData = function (theLanguage, theData) {
        if (theLanguage in _myLanguageData) {
            Logger.info("overwrite i18n data for item " + Public.name + " in language " + theLanguage);
        }
        _myLanguageData[theLanguage] = theData;
        var myEvent = Protected.createEvent(theLanguage);
        Public.dispatchEvent(myEvent);
    };

};

spark.I18nText = spark.ComponentClass("I18nText");

spark.I18nText.Constructor = function (Protected) {
    var Public = this;
    var Base = {};

    Public.Inherit(spark.I18nItem);

    Base.createEvent = Protected.createEvent;
    Protected.createEvent = function (theLanguage) {
        var myEvent = Base.createEvent(theLanguage);
        myEvent.text = Public.text;
        myEvent.fontStyle = Public.fontStyle;
        return myEvent;
    };

    Public.__defineGetter__("text", function () {
        var myData = Public.getLanguageData(Public.language);
        if (myData === null) {
            return "";
        } else {
            return myData;
        }
        // see if this can be replaced by
        // return Public.getLanguageData(Public.language) || "";
    });
		
    Public.__defineGetter__("fontStyle", function() {
        var myFontStyleNode = Public.getLanguageNode(Public.language);
        return myFontStyleNode;
    });
};

spark.I18nImage = spark.ComponentClass("I18nImage");

spark.I18nImage.Constructor = function (Protected) {
    var Public = this;
    var Base = {};

    Public.Inherit(spark.I18nItem);

    Base.createEvent = Protected.createEvent;
    Protected.createEvent = function (theLanguage) {
        var myEvent = Base.createEvent(theLanguage);
        myEvent.src = Public.src;
        return myEvent;
    };

    Public.__defineGetter__("src", function () {
        var myData = Public.getLanguageData(Public.language);
        if (myData === null) {
            return "";
        } else {
            return myData;
        }
        // see if this can be replaced by
        // return Public.getLanguageData(Public.language) || "";
    });
};

spark.I18nMovie = spark.ComponentClass("I18nMovie");

spark.I18nMovie.Constructor = function (Protected) {
    var Public = this;
    var Base = {};

    Public.Inherit(spark.I18nItem);
    
    Base.createEvent = Protected.createEvent;
    Protected.createEvent = function (theLanguage) {
        var myEvent = Base.createEvent(theLanguage);
        myEvent.src = Public.src;
        return myEvent;
    };

    Public.__defineGetter__("src", function () {
        var myData = Public.getLanguageData(Public.language);
        if (myData === null) {
            return "";
        } else {
            return myData;
        }
        // see if this can be replaced by
        // return Public.getLanguageData(Public.language) || "";
    });
};

spark.I18nAudio = spark.ComponentClass("I18nAudio");

spark.I18nAudio.Constructor = function (Protected) {
    var Public = this;
    var Base = {};

    Public.Inherit(spark.I18nItem);
    
    Base.createEvent = Protected.createEvent;
    Protected.createEvent = function (theLanguage) {
        var myEvent = Base.createEvent(theLanguage);
        myEvent.src = Public.src;
        return myEvent;
    };

    Public.__defineGetter__("src", function () {
        var myData = Public.getLanguageData(Public.language);
        if (myData === null) {
            return "";
        } else {
            return myData;
        }
        // see if this can be replaced by
        // return Public.getLanguageData(Public.language) || "";
    });
};
*/
