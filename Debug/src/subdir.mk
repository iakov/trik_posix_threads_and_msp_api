################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/trik_posix_threads_2.cpp \
../src/usbMSP430Interface.cpp 

OBJS += \
./src/trik_posix_threads_2.o \
./src/usbMSP430Interface.o 

CPP_DEPS += \
./src/trik_posix_threads_2.d \
./src/usbMSP430Interface.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

