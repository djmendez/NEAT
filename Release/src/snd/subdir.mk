################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/snd/SoundMgr.cpp 

OBJS += \
./src/snd/SoundMgr.o 

CPP_DEPS += \
./src/snd/SoundMgr.d 


# Each subdirectory must supply rules for building sources it contributes
src/snd/%.o: ../src/snd/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I"/home/daniel/Finalfastecslent/FinalFE/includes/micro" -I/usr/include/OGRE -I/usr/include/boost -I/usr/local/include/OGRE -I"/home/daniel/Finalfastecslent/FinalFE/includes/ai" -I"/home/daniel/Finalfastecslent/FinalFE/includes/gfx" -I"/home/daniel/Finalfastecslent/FinalFE/includes/snd" -I"/home/daniel/Finalfastecslent/FinalFE/includes/tactical" -I"/home/daniel/Finalfastecslent/FinalFE/includes/net" -I/usr/include/OIS -I"/home/daniel/Finalfastecslent/FinalFE/includes" -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


