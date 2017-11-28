################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/gfx/DebugDrawer.cpp \
../src/gfx/GraphicsInteractionManager.cpp \
../src/gfx/cameraMgr.cpp \
../src/gfx/controlGroupsHandler.cpp \
../src/gfx/creationMouseHandler.cpp \
../src/gfx/gfxThread.cpp \
../src/gfx/lobbyMgr.cpp \
../src/gfx/minimap.cpp \
../src/gfx/selectionBox.cpp \
../src/gfx/uiMgr.cpp \
../src/gfx/widgetMgr.cpp \
../src/gfx/widgets.cpp 

OBJS += \
./src/gfx/DebugDrawer.o \
./src/gfx/GraphicsInteractionManager.o \
./src/gfx/cameraMgr.o \
./src/gfx/controlGroupsHandler.o \
./src/gfx/creationMouseHandler.o \
./src/gfx/gfxThread.o \
./src/gfx/lobbyMgr.o \
./src/gfx/minimap.o \
./src/gfx/selectionBox.o \
./src/gfx/uiMgr.o \
./src/gfx/widgetMgr.o \
./src/gfx/widgets.o 

CPP_DEPS += \
./src/gfx/DebugDrawer.d \
./src/gfx/GraphicsInteractionManager.d \
./src/gfx/cameraMgr.d \
./src/gfx/controlGroupsHandler.d \
./src/gfx/creationMouseHandler.d \
./src/gfx/gfxThread.d \
./src/gfx/lobbyMgr.d \
./src/gfx/minimap.d \
./src/gfx/selectionBox.d \
./src/gfx/uiMgr.d \
./src/gfx/widgetMgr.d \
./src/gfx/widgets.d 


# Each subdirectory must supply rules for building sources it contributes
src/gfx/%.o: ../src/gfx/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I"/home/daniel/Finalfastecslent/FinalFE/includes/micro" -I/usr/include/OGRE -I/usr/include/boost -I/usr/local/include/OGRE -I"/home/daniel/Finalfastecslent/FinalFE/includes/ai" -I"/home/daniel/Finalfastecslent/FinalFE/includes/gfx" -I"/home/daniel/Finalfastecslent/FinalFE/includes/snd" -I"/home/daniel/Finalfastecslent/FinalFE/includes/tactical" -I"/home/daniel/Finalfastecslent/FinalFE/includes/net" -I/usr/include/OIS -I"/home/daniel/Finalfastecslent/FinalFE/includes" -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


