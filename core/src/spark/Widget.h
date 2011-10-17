#ifndef _included_mobile_spark_Widget_
#define _included_mobile_spark_Widget_

#include <string>

#include <masl/Logger.h>
#include <masl/MatrixStack.h>
#include <masl/Callback.h>
#include <animation/PropertyAnimation.h>

#include "I18nConstants.h"
#include "Container.h"
#include "Event.h"

namespace spark {

    class I18nItem;
    typedef masl::Ptr<I18nItem> I18nItemPtr;
    class I18nContext;
    typedef masl::Ptr<I18nContext> I18nContextPtr;

    class Widget : public Container  {
    public:
        Widget(const BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
        virtual ~Widget() = 0;
        virtual void prerender(MatrixStack& theCurrentMatrixStack);
        virtual void render(const matrix & theProjectionMatrix) const;
        virtual void realize();

        inline void setVisible(bool theFlag) { AC_DEBUG << "setvisible of " << *this << " to " << theFlag; _visible = theFlag;};
        inline void setSensible(bool theFlag) { _sensible = theFlag;};
        inline bool isVisible() const { return _visible;};
        inline bool isSensible() const { return _sensible;};
        bool isRendered() const;
        float getX() const { return _x;};
        float getY() const { return _y;};
        float getZ() const { return _z;};
        float getScaleX() { return _scaleX;};
        float getRotationX() const {return _rotationX;};
        float getRotationY() const {return _rotationY;};
        float getRotationZ() const {return _rotationZ;};
        void setX(const float theX) { _x = theX; updateMatrix();};
        void setY(const float theY) { _y = theY; updateMatrix();};
        void setZ(const float theZ) { _z = theZ; updateMatrix();};
        void setScaleX(const float theScaleX) { _scaleX = theScaleX; updateMatrix();};
        void setScaleY(const float theScaleY) { _scaleY = theScaleY; updateMatrix();};
        void setScaleZ(const float theScaleZ) { _scaleZ = theScaleZ; updateMatrix();};
        void setRotationX(const float theRotationX) { _rotationX = theRotationX; updateMatrix();};
        void setRotationY(const float theRotationY) { _rotationY = theRotationY; updateMatrix();};
        void setRotationZ(const float theRotationZ) { _rotationZ = theRotationZ; updateMatrix();};

        float getAlpha() const { return _alpha;};
        inline void setAlpha(const float theAlpha) { _alpha = theAlpha; propagateAlpha();};

        const I18nContextPtr getI18nContext() const { return _myI18nContext; };
        LANGUAGE getLanguage() const;
        void switchLanguage(LANGUAGE theLanguage);
        I18nItemPtr getI18nItem() const { return _myI18nItem; }
        matrix _myLocalMatrix; //scale, roation and translation of this node
        
    protected:
        virtual void build() {
            AC_DEBUG << "build widget " << *this;
            _myDirtyFlag = false;
        };
        float getActualAlpha() const { return _actualAlpha;};
        float getParentAlpha() const;
        virtual void propagateAlpha();
        
        matrix _myWorldMVMatrix;
        bool _myDirtyFlag;

        I18nContextPtr _myI18nContext;
        I18nItemPtr _myI18nItem;

        std::vector<I18nContextPtr> getI18nContexts() const ; 
        I18nItemPtr getI18nItemByName(const std::string & theName) const;

    private:
        void setI18nContextIfAvailable();
        void updateMatrix();

        float _x,_y,_z;
        float _scaleX, _scaleY, _scaleZ;
        float _rotationX, _rotationY, _rotationZ;
        float _alpha;
        float _actualAlpha;
        bool _visible;
        bool _sensible;
    };

    typedef masl::Ptr<Widget> WidgetPtr;
    typedef masl::WeakPtr<Widget> WidgetWeakPtr;

    //animations
    typedef void (Widget::* WidgetPropertySetterFunction)(float);
    typedef void (Widget::* WidgetMemberFunction)();
    typedef animation::PropertyAnimation<WidgetWeakPtr, WidgetPropertySetterFunction> WidgetPropertyAnimation;
    typedef masl::Ptr<WidgetPropertyAnimation>  WidgetPropertyAnimationPtr;
    typedef masl::Ptr<masl::MemberFunctionCallback<Widget, WidgetWeakPtr> > WidgetCallbackPtr;
    typedef MemberFunctionEventCallback<Widget, WidgetWeakPtr> WidgetEventCallback;
    typedef masl::Ptr<MemberFunctionEventCallback<Widget, WidgetWeakPtr> > WidgetEventCallbackPtr;
};
#endif
