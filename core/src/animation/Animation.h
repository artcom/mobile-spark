#ifndef _ac_mobile_animation_Animation_h_included_
#define _ac_mobile_animation_Animation_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>

namespace animation {
    
    class Animation;
    typedef boost::shared_ptr<Animation> AnimationPtr;
    class Animation {
    public: 
        Animation(const long theDuration = 1000);
        ~Animation();
        void play(const long theStartTime);
        virtual void doFrame(const long theTime);
        virtual void finish();

        bool isRunning() const { return _myRunning; };
        bool isFinished()  const { return _myFinished; };
    
        void setLoop(const bool theLoop) { _myLoop = theLoop; };

    protected:
        const long _myDuration; //millisecs
        long _myStartTime;
        float _myProgress;

    private:
        unsigned int _myId;
        bool _myRunning;
        bool _myFinished;
        bool _myLoop;
        AnimationPtr _myParent;

        static unsigned int idCounter;
    };
};

#endif
