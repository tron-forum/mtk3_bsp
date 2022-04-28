################################################################################
# micro T-Kernel 3.0 BSP  makefile
################################################################################

OBJS += \
./mtkernel_3/device/adc/sysdepend/stm32h7/adc_stm32h7.o 

C_DEPS += \
./mtkernel_3/device/adc/sysdepend/stm32h7/adc_stm32h7.d 


mtkernel_3/device/adc/sysdepend/stm32h7/%.o: ../device/adc/sysdepend/stm32h7/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
