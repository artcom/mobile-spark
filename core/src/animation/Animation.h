#ifndef _ac_mobile_animation_Animation_h_included_
#define _ac_mobile_animation_Animation_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>

namespace animation {
    
    class Animation;
    typedef boost::shared_ptr<Animation> AnimationPtr;
    class Animation {
    public: 
        void play();
        void doFrame(const unsigned long long theTime);

        bool isRunning() const { return _myRunning; };

    private:
        bool _myRunning;
        bool _myLoop;
        bool _myFinished;
        AnimationPtr _myParent;
        unsigned long long _myDuration; //millisecs
    };
};

#endif
