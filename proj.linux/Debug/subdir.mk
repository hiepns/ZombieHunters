################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../main.o 

CPP_SRCS += \
../main.cpp 

OBJS += \
./main.o 

CPP_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLINUX -I../../Classes -I../../../ -I../../../../cocos2dx -I../../../../cocos2dx/include -I../../../../cocos2dx/kazmath/include -I../../../../cocos2dx/platform/linux -I../../../../cocos2dx/platform/third_party/linux -I../../../../cocos2dx/platform/third_party/linux/glew-1.7.0/glew-1.7.0/include -I../../../../CocosDenshion/include -I/home/hiepns/cocos2d-x/external -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" -Wl,-rpath,/cocos2dx/proj.linux/Debug -Wl,-rpath,/CocosDenshion/proj.linux/Debug,-rpath,/CocosDenshion/third_party/fmod/api/lib
	@echo 'Finished building: $<'
	@echo ' '


