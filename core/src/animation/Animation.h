#ifndef _ac_mobile_animation_Animation_h_included_
#define _ac_mobile_animation_Animation_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace animation {
    
    class Animation;
    typedef boost::shared_ptr<Animation> AnimationPtr;
    typedef boost::weak_ptr<Animation> AnimationWeakPtr;
    class Animation : public boost::enable_shared_from_this<Animation> {
    public: 
        Animation(const long theDuration = 1000);
        virtual ~Animation();
        virtual void doFrame(const long theTime);

        virtual void play(const long theStartTime, const bool theComeToAnEndFlag = false);
        virtual void finish(const long theTime);
        virtual void cancel();

        bool isRunning() const { return _myRunning; };
        bool isFinished()  const { return _myFinished; };
    
        void setLoop(const bool theLoop) { _myLoop = theLoop; };
        void setParent(AnimationWeakPtr theParent) { _myParent = theParent; };
        long getDuration() const { return _myDuration;};
        unsigned int getId() const { return _myId; };

    protected:
        virtual void finishAnimation(const long theTime);
        long _myDuration; //millisecs
        long _myStartTime;
        float _myProgress;

    private:
        unsigned int _myId;
        bool _myRunning;
        bool _myFinished;
        bool _myLoop;
        AnimationWeakPtr _myParent;

        static unsigned int idCounter;
    };
};

#endif
