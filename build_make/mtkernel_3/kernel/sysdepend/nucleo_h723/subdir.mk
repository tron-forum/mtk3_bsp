################################################################################
# micro T-Kernel 3.0 BSP makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysdepend/nucleo_h723/devinit.o \
./mtkernel_3/kernel/sysdepend/nucleo_h723/hw_setting.o \
./mtkernel_3/kernel/sysdepend/nucleo_h723/power_save.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/nucleo_h723/devinit.d \
./mtkernel_3/kernel/sysdepend/nucleo_h723/hw_setting.d \
./mtkernel_3/kernel/sysdepend/nucleo_h723/power_save.d 

mtkernel_3/kernel/sysdepend/nucleo_h723/%.o: ../kernel/sysdepend/nucleo_h723/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
