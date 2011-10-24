#!/bin/bash
#sed -i '1 i\#abc\n#def\n#ghi' test.txt
#sed -i '0,/^#/ d' test.txt

echo "insert licences"


function InsertLicence {
    if [[ $1 == *.cpp || $1 == *.h || $1 == *.mm || $1 == *.m || $1 == *.java || $1 == *.glsl ]];then
        echo "insert licence in $1 with // comment style"
        sed -i '1 i\
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __\
//\
// Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>\
//\
// It is distributed under the Boost Software License, Version 1.0.\
// (See accompanying file LICENSE_1_0.txt or copy at\
//  http://www.boost.org/LICENSE_1_0.txt)\
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __\
' $1
    elif [[ $1 == *.cmake || $1 == *.txt || $1 == *.obj || $1 == *.mtl ]];then   #$1 == *.sh
        echo "insert licence in $1 with # comment style"
        sed -i '1 i\
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __\
#\
# Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>\
#\
# It is distributed under the Boost Software License, Version 1.0.\
# (See accompanying file LICENSE_1_0.txt or copy at\
#  http://www.boost.org/LICENSE_1_0.txt)\
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __\
' $1
    elif [[ $1 == *.spark ]]; then   # $1 == *.xml ||
        echo "insert licence in $1 with <!-- comment style -->"
        sed -i '1 i\
<!-- __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __-->\
<!---->\
<!-- Copyright (C) 1993-2011, ART+COM AG Berlin, Germany <www.artcom.de>-->\
<!---->\
<!-- It is distributed under the Boost Software License, Version 1.0.-->\
<!-- (See accompanying file LICENSE_1_0.txt or copy at-->\
<!--  http://www.boost.org/LICENSE_1_0.txt)-->\
<!-- __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __-->\
' $1
    else
        echo "  ignore $1"
    fi
}

function ProcessDir {
    for f in "$@"
    do
        if [ -d "$f" ]; then   # recursively travel the subdir
            if [[ $f != netsrc && $f != _build && $f != bin && $f != gen && $f != lib ]]; then #netsrc is code from others
                echo "process folder $f"
                cd $f
                ProcessDir $(ls)
                cd ..
            else
                echo "  ignore folder $f"
            fi
        elif [ -a "$f" ]; then # a regular file
            InsertLicence $f
        fi
    done
}

CUR_PATH=$PWD
if [ -n "$1" ]; then
    cd $1
fi
ProcessDir .
cd $CUR_PATH



