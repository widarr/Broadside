#!/bin/bash
BROADSIDEPATH=.
export LD_LIBRARY_PATH=$BROADSIDEPATH/libcgutils/build:$LD_LIBRARY_PATH
$BROADSIDEPATH/Broadside
