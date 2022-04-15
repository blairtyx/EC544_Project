################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

OBJS += \
./component/lists/fsl_component_generic_list.o 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DPRINTF_ADVANCED_ENABLE=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DFRDM_K64F -DFREEDOM -DMCUXPRESSO_SDK -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\board" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\source" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\drivers" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\accel" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\utilities" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\device" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\component\uart" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\component\lists" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\CMSIS" -I"C:\Users\hxpHerman\Documents\MCUXpressoIDE_11.5.0_7232\workspace\frdmk64f_bubble_peripheral\frdmk64f\demo_apps\bubble_peripheral" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


