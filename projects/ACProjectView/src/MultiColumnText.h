#ifndef _included_mobile_acprojectview_MultiColumnText_
#define _included_mobile_acprojectview_MultiColumnText_

#include <spark/Event.h>
#include <spark/Text.h>
#include <spark/Transform.h>

#include <vector>

namespace acprojectview {

    class MultiColumnText : public spark::Transform {
        public: 
            MultiColumnText(const spark::BaseAppPtr theApp, const masl::XMLNodePtr theXMLNode);
            virtual ~MultiColumnText();
            
            static const char * const SPARK_TYPE;             
            virtual const char * const & getType() const { return MultiColumnText::SPARK_TYPE;};

            //void setText(const std::string & theText);
            void setMaxWidth(int theMaxWidth);
            void setMaxHeight(int theMaxHeight);
            void setI18nId(const std::string & theNewI18nId);
            const int & getColumnSpace() const { return _myColumnSpace; };
            void swipe(int theDir);                
            void realize();
            void reset();
                    
        private:
            void onAnimationFinished();
            spark::TextPtr createNewColumn();
            std::vector<spark::TextPtr> _myTextPtrs;
            int _myColumnSpace;
            int _myIniitalXPos;
            int _myVisibleColumnIndex;
            int _myColumnCount;
            bool _myAnimatingFlag;
            
    };
    typedef masl::Ptr<MultiColumnText>MultiColumnTextPtr;
    typedef spark::MemberFunctionEventCallback<MultiColumnText, MultiColumnTextPtr> MultiColumnTextCB;
        
};

#endif
