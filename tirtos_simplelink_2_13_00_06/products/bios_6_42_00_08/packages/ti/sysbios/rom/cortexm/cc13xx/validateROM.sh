#!/bin/bash
#
#  ======== validateROM ========
#
#  Compare the newly built ROM with the golden ROM and
#  generate an error if a mismatch is found.
#

TMPDIR=$(pwd)/ValidateROM
mkdir $TMPDIR

DEVICE=CC13xx
ROMDIR=$TMPDIR/$DEVICE

mkdir $ROMDIR

#  Compare new sysbios.lib and golden sysbios.lib's function list
objdump -t  golden/$DEVICE/sysbios.aem3 | grep "g     F .text" | sort > golden/$DEVICE/sysbios_lib_funcs
objdump -t  $DEVICE/sysbios.aem3 | grep "g     F .text" | sort > $DEVICE/sysbios_lib_funcs
diff golden/$DEVICE/sysbios_lib_funcs $DEVICE/sysbios_lib_funcs > $ROMDIR/libDiff

read line <$ROMDIR/libDiff

if [ -n "$line" ]; then
    echo Error $DEVICE ROM LIBRARY VALIDATION FAILED: New $DEVICE ROM library does not match the golden ROM library
    echo diffs:
    echo
    cat $ROMDIR/libDiff
    echo 
    echo If new $DEVICE/sysbios.aem3 library is ok, copy it to golden/$DEVICE
    rm -rf $TMPDIR
    exit 1
fi

echo $DEVICE ROM VALIDATION PASSED

# copy newly validated sysbios.aem3 library to golden dir
cp $DEVICE/sysbios.aem3 golden/$DEVICE/sysbios.aem3

# commit new validated golden library
if [[ $USER == "xlibrary" ]]
then
    lockFlag=1;
    while [ $lockFlag -eq 1 ]
    do
        while [ -e ../../../../../.git/index.lock ]
        do
            sleep 1
        done
        git ls-files -m | grep golden/$DEVICE/sysbios.aem3
        if [ $? -eq 0 ]
        then
            git commit golden/$DEVICE/sysbios.aem3 -m "Latest $DEVICE sysbios.aem3 library"
            if [ $? -eq 0 ]
            then
                lockFlag=0;
            fi
        else
            lockFlag=0;
        fi
    done
fi

#  Cleanup before exiting
rm -rf $TMPDIR
exit 0
