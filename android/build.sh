#! /bin/bash

# build native
ndk-build 

# update android project
android update project --name mobile-spark --path . 

# build java
ant compile

# build apk && upload
ant install
