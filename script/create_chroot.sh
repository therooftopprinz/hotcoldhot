#!/bin/bash

chr=$1
bin=$2

function copy_libs(){
    list="$(ldd $1 | egrep -o '/lib.*\.so.* ')"
    for i in $list; do cp -v --parents "$i" "${chr}"; done
}

bindir=$(dirname $bin)

mkdir -p $chr/$bindir
mkdir -p $chr/bin
mkdir -p $chr/proc
mkdir -p $chr/sys
mkdir -p $chr/dev
mkdir -p $chr/usr/bin

cp $bin $chr/$bin
cp -v /bin/{bash,touch,ls,rm,find,ldd} $chr/bin
cp -v /usr/bin/{mv,cp,grep} $chr/usr/bin

copy_libs $bin
copy_libs /bin/bash
copy_libs /bin/touch
copy_libs /bin/ls
copy_libs /bin/rm
copy_libs /bin/find
copy_libs /bin/ldd
copy_libs /usr/bin/grep
copy_libs /usr/bin/cp
copy_libs /usr/bin/mv


mount -t proc /proc $chr/proc/
mount --rbind /sys $chr/sys/
mount --rbind /dev $chr/dev/

chroot $chr /bin/bash

mount --make-rslave $chr/sys/
mount --make-rslave $chr/dev/

umount -fR $chr/proc/
umount -fR $chr/sys/
umount -fR $chr/dev/

rm -rf $chr