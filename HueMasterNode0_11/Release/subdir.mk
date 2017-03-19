################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
../HueMasterNode0_11.ino 

CPP_SRCS += \
../.ino.cpp 

LINK_OBJ += \
./.ino.cpp.o 

INO_DEPS += \
./HueMasterNode0_11.ino.d 

CPP_DEPS += \
./.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ../.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/rayt/sloeber//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/cores/arduino" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/variants/mega" -I"/home/rayt/sloeber/arduinoPlugin/libraries/Adafruit_CC3000_Library/1.0.3" -I"/home/rayt/sloeber/arduinoPlugin/libraries/Adafruit_CC3000_Library/1.0.3/utility" -I"/home/rayt/gitlab/libraries/CountUpDownTimer-master" -I"/home/rayt/sloeber/arduinoPlugin/libraries/LiquidCrystal/1.0.4" -I"/home/rayt/sloeber/arduinoPlugin/libraries/LiquidCrystal/1.0.4/src" -I"/home/rayt/gitlab/libraries/Time" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/Wire" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/Wire/src" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/SPI" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/SPI/src" -I"/home/rayt/gitlab/libraries/MemoryFree" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

HueMasterNode0_11.o: ../HueMasterNode0_11.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/rayt/sloeber//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/cores/arduino" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/variants/mega" -I"/home/rayt/sloeber/arduinoPlugin/libraries/Adafruit_CC3000_Library/1.0.3" -I"/home/rayt/sloeber/arduinoPlugin/libraries/Adafruit_CC3000_Library/1.0.3/utility" -I"/home/rayt/gitlab/libraries/CountUpDownTimer-master" -I"/home/rayt/sloeber/arduinoPlugin/libraries/LiquidCrystal/1.0.4" -I"/home/rayt/sloeber/arduinoPlugin/libraries/LiquidCrystal/1.0.4/src" -I"/home/rayt/gitlab/libraries/Time" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/Wire" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/Wire/src" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/SPI" -I"/home/rayt/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.17/libraries/SPI/src" -I"/home/rayt/gitlab/libraries/MemoryFree" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


