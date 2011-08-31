#include "EventFactory.h"


#include <masl/Logger.h>
#include <masl/XMLNode.h>

using namespace masl;

namespace spark {
    
    //TODO: decentralize this again
    void EventFactory::setupFactory() {
        bool registered = registerEvent("StageEvent", spark::createStageEvent);
        registered = registerEvent("TouchEvent", spark::createTouchEvent);
    };
    
    EventFactory::EventFactory() {
        setupFactory();
    }
    EventFactory::~EventFactory() {}

    // Returns 'true' if registration was successful
    bool EventFactory::registerEvent(const std::string & theEventName,
                    const CreateEventCallback theCreateFn) {
        return _myCreateCallbackMap.insert(CallbackMap::value_type(theEventName, theCreateFn)).second;
    }

    // Returns 'true' if the ShapeId was registered before
    bool EventFactory::unregisterEvent(const std::string & theEventName) {
        return _myCreateCallbackMap.erase(theEventName) == 1;
    }

    EventPtr EventFactory::handleEvent(const std::string & theEventString) const {
        //AC_PRINT << "handle event : " << theEventString;
        xmlParserCtxtPtr ctxt; 
        xmlDocPtr doc;
        ctxt = xmlNewParserCtxt();
        doc = xmlCtxtReadMemory(ctxt, theEventString.c_str(), strlen(theEventString.c_str()), "", NULL, XML_PARSE_DTDATTR);
        xmlNode *root_node = NULL;
        root_node = xmlDocGetRootElement(doc);
        XMLNodePtr myXMLNode(new XMLNode(root_node));
        //AC_PRINT<<"handleEvent "<<*myXMLNode;
        CallbackMap::const_iterator i = _myCreateCallbackMap.find(myXMLNode->nodeName);
        if (i == _myCreateCallbackMap.end()) {
            throw EventFactoryException("Unknown Event: " + myXMLNode->nodeName, PLUS_FILE_LINE);
        }
        xmlFreeDoc(doc);        
        return (i->second)(myXMLNode);
    }

}
