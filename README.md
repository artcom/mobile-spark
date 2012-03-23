# Spark

___

## Was ist Spark?

Spark ist ein graphikorientiertes Framework for iOS und Android. Es basiert
auf openGL ES2.0 und ermöglicht durch seinen Komponentenaufbau eine produktive
Anwendungsentwicklung. Das strukturelle Layout wird durch XML-Dateien
beschrieben, während das Verhalten der Komponenten durch optionalen C++
Sourcecode implementiert wird.  Der Spark-Anwendungscode ist
plattformübergreifend, d.h. ein Spark-Projekt kann sowohl Android- als auch
iOS-Applikationen erzeugen.

**Merkmale:**

* XML-Layout-Dateien mit C++ Implementierungen
* 2D- und 3D-Unterstützung
* Widget- und Komponentensystem
* Event-System
* obj-Loader
* Sound und Animation
* HTTP-Konnektivität
* unterstützt Sensoren und Camera

___

## Voraussetzungen

### für Android:

* Hardware: Androidgerät mit openGL 2.0 Unterstützung 
* Android-Version: 2.3 (Gingerbread) und höher
* Entwicklungssystem: Linux, Windows, Mac OSX
* Software: Android SDK Rev.9, Android NDK, GIT, cygwin(nur Windows), C++ Boost Libraries, beliebiger Editor oder IDE

### für iOS:

* Hardware: iPad, iPad2, iPhone 3GS oder höher 
* iOS-Version: 3.0 und höher
* Entwicklungssystem: Mac OSX
* Software: XCode 5.0, GIT, C++ Boost Libraries
* sonstiges: iOS Developer Account (für Deployment)

___


missing:
unzip boost in netsrc

## Installation

### Linux / Mac

* erstelle neuen Spark-Ordner

    $ cd ~/dev
    $ mkdir mobile-spark 
    $ cd mobile-spark

* checkout Repository: 

    $ git clone git://github.com/artcom/mobile-spark.git

* setze System-Suchpfade in @.bash_profile@ bzw. @.bashrc@ für
  Android-SDK/tools und Android-SDK/platform-tools, z.B.:
<pre>
$ export PATH=~/Library/android-sdk-mac_x86-1.6_r1/tools:$PATH
$ export PATH=~/Library/android-sdk-mac_x86-1.6_r1/platform-tools:$PATH
</pre>

* setze Umgebungsvariablen in **.bash\_profile** bzw. **.bashrc** für
  ANDROID\_NDK, ANDROID\_NDK\_TOOLCHAIN\_ROOT, MOBILE\_SPARK. z.B.:
<pre>
$ export ANDROID_NDK=~/Library/android-ndk-r6b
$ export ANDROID_NDK_TOOLCHAIN_ROOT=$ANDROID_NDK/android-toolchain
$ export MOBILE_SPARK=~/dev/mobile-spark
</pre>
### Windows

* falls nicht vorhanden, installiere Cygwin
* setze System-Suchpfade für:
 * msys git
 * cmake
 * ANDROID_NDK
 * ANDROID_SDK/tools
 * ANDROID_SDK/platform-tools
 * ANT_HOME (jakarta ant)
* setze Umgebungsvariablen für
 * ANDROID_NDK (inkl. /tools und /platform-tools)
 * ANDROID_HOME (für Android SDK)
 * ANT_HOME
 * JAVA_HOME
 * BOOST_HOME
* Build android ndk toolchain
<pre>
$ set "MOBILE_SPARK" to your mobile-spark directory (in windows style!!!)
$ export NDK_TMPDIR=$(cygpath "$ANDROID_NDK/")../ndk-tmp
$ call $ANDROID_NDK/build/tools/make-standalone-toolchain.sh \
        --platform=android-9 \
        --install-dir=$ANDROID_NDK/android-toolchain \
        --system=windows
$ export ANDROID_NDK_TOOLCHAIN_ROOT=$ANDROID_NDK/android-toolchain
</pre>

* erstelle mobile-spark Ordner
* checkout Repository im mobile-spark Ordner:

    $ git clone git://github.com/artcom/mobile-spark.git

___

## OS specific builds

### Android

    $ cd $MOBILE_SPARK/android/
    $ ./build.sh

### iOS

create and open XCode project:
<pre>
$ cd $MOBILE_SPARK/iOS/
$ rm -rf _build/
$ mkdir _build
$ ./xcode_ios.sh
$ open mobile-spark.xcodeproj
</pre>

Einstellungen in XCode IDE:

* TARGET-Scheme: SparkViewerBase > iOS Device
* Build Settings: _Target Device Family -> iPhone_ oder _iPad_
* Build Settings: _iOS Deployment Target -> iOS 4.1_

* Bauen: _Product -> Build_

___


## Teste Demo

Zum Testen der DemoApp müssen die mobile-spark-Libraries kompiliert sein
(siehe _Build für Android/iOS_).

### Android

* Android-Gerät:
 * aktiviere USB-Debugging
 * erlaube Installation unbekannter Quellen
 * verbinde mittels USB mit Rechner
<pre>
$ cd $MOBILE_SPARK/projects/DemoApp/
$ rm -rf \_build
$ cd android/
$ ./build.sh
</pre>

* copy result to device:
<pre>
$ ./push.sh
$ ./start.sh
</pre>

### iOS

* Registrierung als iOS Developer erforderlich:

> [iOS Developer Center](http://developer.apple.com/devcenter/ios)

* Zur Code-Signierung ist ein Provisioning Profile erforderlich:

> [Creating and Downloading Development Provisioning Profiles](http://developer.apple.com/library/ios/#documentation/ToolsLanguages/Conceptual/DevPortalGuide/CreatingandDownloadingDevelopmentProvisioningProfiles/CreatingandDownloadingDevelopmentProvisioningProfiles.html#//apple_ref/doc/uid/TP40011159-CH23-SW1)

* Das Provisioning Profile muss auf iPad/iPhone installiert werden
  (ausgenommen inHouse provisioning profiles)
* verbinde Gerät via USB
* goto to project folder and create XCode project:
<pre>
$ cd $MOBILE_SPARK/projects/DemoApp/ios/
$ mkdir _build
$ cd _build/
$ rm -rf *
$ ../xcode_ios.sh
$ open DemoApp.xcodeproj
</pre>

Einstellungen in XCode IDE:

* TARGET-Scheme: DemoAppApp > yourDevice
* Build Settings: _TARGETS -> DemoAppApp -> Target Device Family -> iPhone_ oder _iPad_
* Build Settings: _TARGETS -> DemoAppApp -> iOS Deployment Target -> iOS 4.1_
* Build Settings: _TARGETS -> DemoAppApp -> Code Signing Identity -> yourProvisioningProfile_

* Bauen: _Product -> Build_
* Deployment und Ausführung: _Product -> Run_

___

## Neues Project anlegen

* lege an beliebiger Stelle workspace an

    $ cd anyWhere
    $ mkdir myWorkspace
    $ cd myWorkspace

* rufe createProject-Skript auf (Achtung! Projektnamen dürfen keine
  Sonderzeichen beinhalten)

    $ $MOBILE_SPARK/createProject.sh MyNewProjectName

* passe __relativen__ Pfad in
  _MyNewProjectName/android/MyNewProjectName/_**build.properties** an: 
* relativer Pfad muss von _MyNewProjectName/android/MyNewProjectName/_ nach
  _$MOBILE\_SPARK/\_build/lib/_ zeigen, z.B.:

    native.libs.dir --> ../../../mobile-spark/_build/lib

___

## Tutorials


### Erste Schritte

Folgendes Programm zeigt den Umgang mit Spark-Komponenten, EventListenern und
einfachen Animationen.

![](https://redmine.intern.artcom.de/attachments/980/HelloWorld-Screenshot.jpeg)


1. kompiliere mobile-spark für Android bzw. iOS

* erstelle HelloWorld-Projekt

    $ cd myWorkspace
    $ $MOBILE_SPARK/createProject.sh HelloWorld

* passe _build.properties_ an 
* öffne Layout-Datei: _HelloWorld/layouts/main.spark_  

<pre>
&lt;Window name="ProjectView" fullscreen="true" i18nContext="global-i18n" width="800" height="480" orientation="landscape" clearColor="[0,0,0.5,1]"&gt;
    &lt;!-- userinterface is located in 2dworld --&gt;
    &lt;View name="mainView" world="2dworld" pos="[0,0]" size="[1.0,1.0]" cameraName="2dcamera"/&gt;
    &lt;World name="2dworld"&gt;    
        &lt;RenderCamera name="2dcamera" frustum="orthonormal[auto]"/&gt;    

        &lt;Transform name="transform"&gt;
            &lt;Text name="testtextgreen" y="50" x="10" z="13" text="Hello World" color="[0,1,0,1]" fontsize="32"/&gt;
        &lt;/Transform&gt;    
    &lt;/World&gt;        
&lt;/Window&gt;
</pre>

ändere Text Komponente und füge Rechteck hinzu  

    <Transform name="transform" x="50">
        <Text 
            name="blackText" color="[0,0,0,1]" 
            x="20" y="50" z="1" 
            text="push" fontsize="32"/>
        <Rectangle name="whiteRect" color="[1,1,1]"
            x="0" y="0" z="0" width="110" height="2000" />
    </Transform>

öffne Header der Hauptklasse: _HelloWorld/src/HelloWorld.h_  
inkludiere spark-Komponenten: Transform, Text, Rectangle und Window

    #include <spark/Transform.h>
    #include <spark/Text.h>
    #include <spark/Rectangle.h>
    #include <spark/Window.h>

füge privaten Touch-Handler hinzu:

   void onRectTouched(spark::EventPtr theEvent);

deklariere Attribute für Text, Transform, und Displaybreite:

    spark::TransformPtr _myTransform;
    spark::TextPtr _myText;
    int _myWidth;

bearbeite Implementierung der Hauptklasse: _HelloWorld/src/HelloWorld.cpp_  
definiere benötigte Attribute in _setup_-Methode:  
Display-Breite: 

    _myWidth=_mySparkWindow->getSize()[0];

setze Zeiger auf Layout-Komponenten:

    _myTransform = boost::static_pointer_cast<spark::Transform>(
        _mySparkWindow->getChildByName("2dworld", true)->getChildByName(
            "transform",true
        )
    );
    _myText =  boost::static_pointer_cast<spark::Text>(
        _myTransform->getChildByName("blackText",true)
    );
    RectanglePtr myRect = boost::static_pointer_cast<Rectangle::Rectangle>(
        _myTransform->getChildByName("whiteRect",true)
    );
    HelloWorldPtr ptr = boost::static_pointer_cast<HelloWorld>(
        shared_from_this()
    );

erzeuge Callback-Methode für Touch-Event, registriere Listener auf Rechteck:

    spark::EventCallbackPtr myTransformTouchedCB = EventCallbackPtr(new HelloWorldEventCB(ptr,&HelloWorld::onRectTouched));
    myRect->addEventListener(TouchEvent::PICKED, myTransformTouchedCB); 

implementiere Callback-Methode

    void HelloWorld::onRectTouched(EventPtr theEvent) {
         float oldX = _myTransform->getX();
         float newX = std::abs(oldX-_myWidth) - 100;
         
         // switch ping and pong
         if(newX < oldX) {
             _myText->setText("ping");
         } else {
             _myText->setText("pong");
         }
    }

lasse Bewegung des weißen Rechtecks animieren:
inkludiere Animations-Header:

    #include <animation/AnimationManager.h>
    #include <animation/SequenceAnimation.h>
    #include <animation/Easing.h

erweitere Callback-Methode _onRectTouched_, so dass die x-Position des
Transforms animiert wird:

    void HelloWorld::onRectTouched(EventPtr _) {
        ...
        // init animation, here for property position x of _myTransform;
        //
        WidgetWeakPtr myWidget = WidgetWeakPtr(WidgetPtr(_myTransform));
        WidgetPropertyAnimationPtr myAnimation = WidgetPropertyAnimationPtr(
            new WidgetPropertyAnimation(
                myWidget, &Widget::setX, oldX, newX, 500,
                animation::EasingFnc(animation::easeInOutElastic)
            )
        );
         
        // add property animation to playable sequence animation (also
        // possible: ParallelAnimation)
        //
        animation::SequenceAnimationPtr mySequence = 
           animation::SequenceAnimationPtr(
               new animation::SequenceAnimation()
           );
        mySequence->add(myAnimation);
        
        // start animation
        //
        animation::AnimationManager::get().play(mySequence);
    }


## Tips & Tricks

* das Bauen von Android-Apps kann für Multi-Core-Systeme beschleunigt werden:
  Parameter -j(Number of Cores) anhängen:

    $ ./build.sh -j3

* Android-Apps können per Kommandozeile beenden werden

    $ ./kill.sh

* Debugging und Logging unter Android ist mit adb-tools möglich, z.B.:

    $ adb logcat

* ändern sich bei Android nur Texturen, Layout-Dateien, Sounds usw. muss nicht
  neu gebaut werden, es reicht die neuen Daten mittels Push-Skript @./push.sh@
  auf die SD-Card zu kopieren
* bei unerwarteten Fehlern hilft es manchmal, die App manuell vom Gerät zu
  löschen
* bei iOS sollte vor jedem Bauen des Projekt ein Cleaning statt finden
  (cmd+shift+K)
* Debugging ist in iOS nur mit Developer-Provisioning-Profiles möglich


## Architektur


### Klassenhierarchie der Widgets

![](https://redmine.intern.artcom.de/attachments/976/mobileSpark-ClassDiagram.png)


### Klassenhierarchie für Animationen

![](https://redmine.intern.artcom.de/attachments/982/animation-classDiagram.png)

## Referenz

_to be done_
