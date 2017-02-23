#!/bin/sh

CONFIG=./configs/jetson-tk1-rtems-demo.cell
RTEMS_ADDRESS=0x00000000
JH_DIR="/root/workspace/jailhouse"
JH_EXEC="/usr/local/sbin/jailhouse"
REMOTE="root@brian"
SERIAL="/dev/ttyUSB0"
OUTPUT="./results"
TESTSOURCES="/home/andi/workspace/rtems/rtems-gogs/testsuites/psxtmtests"
#TESTSOURCES="$SOURCES$(echo $(pwd)|awk -F \"/\" '{print ( $(NF-1)"/"$(NF) ) }')"

#set -x
#trap read debug

pwd=$(pwd)
shopt -s globstar

ssh "$REMOTE" "mkdir /boot/test" 2>/dev/null

for i in **/*.ralf; do
  echo "Test: $i"
  mkimage -A arm -T kernel -a 0x90000000 -e 0x90000080 -n hello -O rtems -C \
    none -d "$i" hello.uimage || exit 1
  scp hello.uimage root@brian:/boot/test/ && ssh root@brian "reboot"
  find "$TESTSOURCES/$(dirname $i)" -name "*.h" -exec sh -c "pygmentize -g {} | cat -n" \;
  find "$TESTSOURCES/$(dirname $i)" -name "*.c" -exec sh -c "pygmentize -g {} | cat -n" \;

  read waiting
done
