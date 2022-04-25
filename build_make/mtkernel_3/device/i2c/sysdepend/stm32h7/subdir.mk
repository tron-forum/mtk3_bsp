################################################################################
# micro T-Kernel 3.0 BSP  makefile
################################################################################

OBJS += \
./mtkernel_3/device/i2c/sysdepend/stm32h7/i2c_stm32h7.o 

C_DEPS += \
./mtkernel_3/device/i2c/sysdepend/stm32h7/i2c_stm32h7.d 


# Each subdirectory must supply rules for building sources it contributes
mtkernel_3/device/i2c/sysdepend/stm32h7/%.o: ../device/i2c/sysdepend/stm32h7/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
