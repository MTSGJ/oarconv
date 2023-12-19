#!/bin/sh

export LD_LIBRARY_PATH=${HOME}/oarconv/lib:$LD_LIBRARY_PATH

ASSETSDIR=${HOME}/oarconv/assets/:./assets/
EXTRLCOM="${HOME}/oarconv/bin/opj_decompress -i %s -o %s >/dev/null 2>&1"

${HOME}/oarconv/bin/oarconv -a $ASSETSDIR -c "$EXTRLCOM" $*

