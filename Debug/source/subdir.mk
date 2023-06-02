################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/arraysInAssembly.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/arraysInAssembly.d \
./source/semihost_hardfault.d 

OBJS += \
./source/arraysInAssembly.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DPRINTF_FLOAT_ENABLE=0 -DSCANF_FLOAT_ENABLE=0 -DPRINTF_ADVANCED_ENABLE=0 -DSCANF_ADVANCED_ENABLE=0 -DFRDM_K64F -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Xpresso\arraysInAssembly\source" -I"C:\Xpresso\arraysInAssembly\utilities" -I"C:\Xpresso\arraysInAssembly\drivers" -I"C:\Xpresso\arraysInAssembly\device" -I"C:\Xpresso\arraysInAssembly\component\uart" -I"C:\Xpresso\arraysInAssembly\component\lists" -I"C:\Xpresso\arraysInAssembly\CMSIS" -I"C:\Xpresso\arraysInAssembly\board" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/arraysInAssembly.d ./source/arraysInAssembly.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

