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

mkdir "$OUTPUT" 2>/dev/null
ssh "$REMOTE" "mkdir \"$JH_DIR\"/measurements/" 2>/dev/null

ssh "$REMOTE" "sync"
ssh "$REMOTE" "insmod \"$JH_DIR\"/driver/jailhouse.ko"
ssh "$REMOTE" "$JH_EXEC enable \"$JH_DIR\"/configs/jetson-tk1.cell"
for i in **/*.ralf; do
  echo "Test: $i"
	scp "$i" "$REMOTE":"$JH_DIR"/measurements/ || exit 1

	ssh "$REMOTE" "$JH_EXEC cell create \"$JH_DIR\"/\"$CONFIG\""
	ssh "$REMOTE" "$JH_EXEC cell load 1 $JH_DIR/measurements/\"$(basename $i)\" -a 0" || exit 1
  #stty -F "$SERIAL" 115200
  #cat "$SERIAL" | tee "$OUTPUT"/$(basename $i).log 2>/dev/null &
  #pid=$!
	ssh "$REMOTE" "$JH_EXEC cell start 1" || exit 1
  find "$TESTSOURCES/$(dirname $i)" -name "*.h" -exec sh -c "pygmentize -g {} | cat -n" \;
  find "$TESTSOURCES/$(dirname $i)" -name "*.c" -exec sh -c "pygmentize -g {} | cat -n" \;

  read waiting
  #kill -INT $pid 2>/dev/null
  #kill -PIPE $pid 2>/dev/null
  echo "RESULT: "
  #cat "$OUTPUT"/$(basename $i).log
done
#set +x
ssh "$REMOTE" "$JH_EXEC cell shutdown 1" || exit 1
ssh "$REMOTE" "$JH_EXEC disable" || exit 1
ssh "$REMOTE" "rmmod jailhouse" || exit 1
