#ifndef _ac_mobile_animation_Animation_h_included_
#define _ac_mobile_animation_Animation_h_included_

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>

#include <masl/Callback.h>
#include <masl/Settings.h>
#include <masl/Ptr.h>

namespace animation {

    DECLARE_EXCEPTION(AnimationException, masl::Exception);

    typedef boost::function<float(float)> EasingFunctionPtr;
    inline float defaultEasing(float theValue) { return theValue; };

    class Animation;
    typedef masl::Ptr<Animation> AnimationPtr;
    class Animation : public boost::enable_shared_from_this<Animation> {
    public:
        Animation(const masl::UInt64 theDuration = 1000, const EasingFunctionPtr theEasing = defaultEasing);
        virtual ~Animation();
        virtual void doFrame(const masl::UInt64 theTime);

        virtual void play(const masl::UInt64 theStartTime, const bool theComeToAnEndFlag = false);
        virtual void finish(const masl::UInt64 theTime);
        virtual void cancel();

        bool isRunning() const { return _myRunning; };
        bool isFinished()  const { return _myFinished; };

        void setLoop(const bool theLoop) { _myLoop = theLoop; };
        masl::UInt64 getDuration() const { return _myDuration;};
        unsigned int getId() const { return _myId; };

        void setOnPlay(masl::CallbackPtr theOnPlay) { _myOnPlay = theOnPlay; };
        void setOnFinish(masl::CallbackPtr theOnFinish) { _myOnFinish = theOnFinish; };
        void setOnCancel(masl::CallbackPtr theOnCancel) { _myOnCancel = theOnCancel; };

    protected:
        virtual void finishAnimation(const masl::UInt64 theTime);
        masl::UInt64 _myDuration; //millisecs
        masl::UInt64 _myStartTime;
        float _myProgress;
        EasingFunctionPtr _myEasingFunction;

    private:
        float _myProgressTime;
        unsigned int _myId;
        bool _myRunning;
        bool _myFinished;
        bool _myLoop;

        masl::CallbackPtr _myOnPlay;
        masl::CallbackPtr _myOnFinish;
        masl::CallbackPtr _myOnCancel;

        static unsigned int idCounter;
    };
};

#endif
