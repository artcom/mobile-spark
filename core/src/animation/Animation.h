#ifndef _ac_mobile_animation_Animation_h_included_
#define _ac_mobile_animation_Animation_h_included_

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>

#include <masl/Callback.h>

namespace animation {
    
    typedef boost::function<float(float)> EasingFunctionPtr;
    inline float defaultEasing(float theValue) { return theValue; };

    

    class Animation;
    typedef boost::shared_ptr<Animation> AnimationPtr;
    typedef boost::weak_ptr<Animation> AnimationWeakPtr;
    class Animation : public boost::enable_shared_from_this<Animation> {
    public: 
        Animation(const long theDuration = 1000, const EasingFunctionPtr theEasing = defaultEasing);
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

        void setOnPlay(masl::CallbackPtr theOnPlay) { _myOnPlay = theOnPlay; };
        void setOnFinish(masl::CallbackPtr theOnFinish) { _myOnFinish = theOnFinish; };
        void setOnCancel(masl::CallbackPtr theOnCancel) { _myOnCancel = theOnCancel; };

    protected:
        virtual void finishAnimation(const long theTime);
        long _myDuration; //millisecs
        long _myStartTime;
        float _myProgress;
        EasingFunctionPtr _myEasingFunction;

    private:
        float _myProgressTime;
        unsigned int _myId;
        bool _myRunning;
        bool _myFinished;
        bool _myLoop;
        AnimationWeakPtr _myParent;

        masl::CallbackPtr _myOnPlay;
        masl::CallbackPtr _myOnFinish;
        masl::CallbackPtr _myOnCancel;

        static unsigned int idCounter;
    };
};

#endif
