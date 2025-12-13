#!/usr/bin/bash

. /etc/os-release
BASEDIR="$(dirname -- "$(readlink -f -- "$0";)")"

dest=$BASEDIR/build
if [[ -d $dest ]]; then
    rm -rf $dest
fi

need_index="false"

if [[ $VERSION_CODENAME == "trixie" ]]; then
    need_index="true"
fi

meson setup build -Dbuildtype=plain -Dneed_index="$need_index"
ninja -C build
sudo ninja -C build install


