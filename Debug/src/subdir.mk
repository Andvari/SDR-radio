################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DEEMPHASIS.cpp \
../src/FILTER.cpp \
../src/HETERODYNE.cpp \
../src/MARK.cpp \
../src/OSC.cpp \
../src/PHASEDETECTOR.cpp \
../src/PLL.cpp \
../src/RTL.cpp \
../src/SDR-radio.cpp 

OBJS += \
./src/DEEMPHASIS.o \
./src/FILTER.o \
./src/HETERODYNE.o \
./src/MARK.o \
./src/OSC.o \
./src/PHASEDETECTOR.o \
./src/PLL.o \
./src/RTL.o \
./src/SDR-radio.o 

CPP_DEPS += \
./src/DEEMPHASIS.d \
./src/FILTER.d \
./src/HETERODYNE.d \
./src/MARK.d \
./src/OSC.d \
./src/PHASEDETECTOR.d \
./src/PLL.d \
./src/RTL.d \
./src/SDR-radio.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


