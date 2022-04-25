################################################################################
# micro T-Kernel 3.0 BSP makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/stm32h7/int_stm32h7.o \
./mtkernel_3/lib/libtk/sysdepend/cpu/stm32h7/ptimer_stm32h7.o 

C_DEPS += \
./mtkernel_3/lib/libtk/sysdepend/cpu/stm32h7/int_stm32h7.d \
./mtkernel_3/lib/libtk/sysdepend/cpu/stm32h7/ptimer_stm32h7.d 

mtkernel_3/lib/libtk/sysdepend/cpu/stm32h7/%.o: ../lib/libtk/sysdepend/cpu/stm32h7/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
