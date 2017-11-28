################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ExplosionType.cpp \
../src/WeaponType.cpp \
../src/aspect.cpp \
../src/buildings.cpp \
../src/command.cpp \
../src/commandFactory.cpp \
../src/commandHelp.cpp \
../src/distanceMgr.cpp \
../src/engine.cpp \
../src/ent.cpp \
../src/entityMgr.cpp \
../src/gameMgr.cpp \
../src/main.cpp \
../src/mgr.cpp \
../src/physics1.cpp \
../src/physics2.cpp \
../src/physics3.cpp \
../src/physics3DR.cpp \
../src/pyEngine.cpp \
../src/selectionMgr.cpp \
../src/unitAI.cpp \
../src/unitBuilder.cpp \
../src/unitWeapons.cpp \
../src/weaponMgr.cpp 

OBJS += \
./src/ExplosionType.o \
./src/WeaponType.o \
./src/aspect.o \
./src/buildings.o \
./src/command.o \
./src/commandFactory.o \
./src/commandHelp.o \
./src/distanceMgr.o \
./src/engine.o \
./src/ent.o \
./src/entityMgr.o \
./src/gameMgr.o \
./src/main.o \
./src/mgr.o \
./src/physics1.o \
./src/physics2.o \
./src/physics3.o \
./src/physics3DR.o \
./src/pyEngine.o \
./src/selectionMgr.o \
./src/unitAI.o \
./src/unitBuilder.o \
./src/unitWeapons.o \
./src/weaponMgr.o 

CPP_DEPS += \
./src/ExplosionType.d \
./src/WeaponType.d \
./src/aspect.d \
./src/buildings.d \
./src/command.d \
./src/commandFactory.d \
./src/commandHelp.d \
./src/distanceMgr.d \
./src/engine.d \
./src/ent.d \
./src/entityMgr.d \
./src/gameMgr.d \
./src/main.d \
./src/mgr.d \
./src/physics1.d \
./src/physics2.d \
./src/physics3.d \
./src/physics3DR.d \
./src/pyEngine.d \
./src/selectionMgr.d \
./src/unitAI.d \
./src/unitBuilder.d \
./src/unitWeapons.d \
./src/weaponMgr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I"/home/daniel/Finalfastecslent/FinalFE/includes/micro" -I/usr/include/OGRE -I/usr/include/boost -I/usr/local/include/OGRE -I"/home/daniel/Finalfastecslent/FinalFE/includes/ai" -I"/home/daniel/Finalfastecslent/FinalFE/includes/gfx" -I"/home/daniel/Finalfastecslent/FinalFE/includes/snd" -I"/home/daniel/Finalfastecslent/FinalFE/includes/tactical" -I"/home/daniel/Finalfastecslent/FinalFE/includes/net" -I/usr/include/OIS -I"/home/daniel/Finalfastecslent/FinalFE/includes" -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


