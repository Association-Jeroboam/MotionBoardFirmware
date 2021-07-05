docker run --rm --name jeroboam -it --device=/dev/cu.usbmodem141203 --privileged -v $(pwd):/home/dev jeroboam /bin/sh -c "make -j -C firmware/src/hardware/robot"
cd firmware/src/hardware/robot
openocd -s ./cfg -c "set BIN_FILE build/MotionBoard.bin" -f flash.cfg
