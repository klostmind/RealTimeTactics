################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RTT_Ogre_3D.cpp 

OBJS += \
./src/RTT_Ogre_3D.o 

CPP_DEPS += \
./src/RTT_Ogre_3D.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/OGRE -I/usr/include/OGRE/Terrain -I/usr/lib/OGRE -I/usr/include/OIS -I/usr/share/OGRE/Samples/Common/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


