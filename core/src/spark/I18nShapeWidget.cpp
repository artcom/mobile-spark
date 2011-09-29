#include "I18nContext.h"
#include "I18nShapeWidget.h"

namespace spark {
    I18nShapeWidget::I18nShapeWidget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode)
        : ShapeWidget(theApp, theXMLNode) {
        setI18nId(_myXMLNode->getAttributeAs<std::string>("i18nId", ""));
    }

    I18nShapeWidget::~I18nShapeWidget() {}

    void
    I18nShapeWidget::realize()  {
        ShapeWidget::realize();
        I18nShapeWidgetPtr ptr = boost::static_pointer_cast<I18nShapeWidget>(shared_from_this());        
        I18nWidgetEventCallbackPtr myHandleLanguageSwitch = I18nWidgetEventCallbackPtr(new I18nWidgetEventCallback(ptr, &I18nShapeWidget::handleI18nOnLanguageSwitch));
        I18nHandler::realize(boost::static_pointer_cast<Widget>(shared_from_this()), myHandleLanguageSwitch);                     
    }

    void 
    I18nShapeWidget::handleI18nOnLanguageSwitch(const EventPtr theEvent) {
        setI18nData(getI18nItem()->getLanguageData());
        _myDirtyFlag = true;
        AC_DEBUG << "on language switch data " << data_;
    }

}
