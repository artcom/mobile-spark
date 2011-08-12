#ifndef _ac_mobile_animation_Animation_h_included_
#define _ac_mobile_animation_Animation_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>

namespace animation {
    class Animation {
    public: 
        void play();
        void doFrame(const unsigned long long theTime);

        bool isRunning() const { return _myRunning; };

    private:
        bool _myRunning;
    };

    typedef boost::shared_ptr<Animation> AnimationPtr;
};

#endif
