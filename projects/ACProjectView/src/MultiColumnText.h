// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>
//
// It is distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __

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
            int _myInitalXPos;
            std::vector<spark::TextPtr>::size_type _myVisibleColumnIndex;
            std::vector<spark::TextPtr>::size_type _myColumnCount;
            bool _myAnimatingFlag;
            
    };
    typedef masl::Ptr<MultiColumnText>MultiColumnTextPtr;
    typedef masl::WeakPtr<MultiColumnText>MultiColumnTextWeakPtr;
    typedef masl::MemberFunctionCallback<MultiColumnText, MultiColumnTextWeakPtr> MultiColumnTextCB;
    typedef spark::MemberFunctionEventCallback<MultiColumnText, MultiColumnTextWeakPtr> MultiColumnTextEventCB;
};

#endif
