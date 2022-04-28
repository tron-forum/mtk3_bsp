################################################################################
# micro T-Kernel 3.0 BSP makefile
################################################################################

OBJS += \
./mtkernel_3/kernel/sysdepend/cpu/stm32h7/cpu_clock.o \
./mtkernel_3/kernel/sysdepend/cpu/stm32h7/vector_tbl.o 

C_DEPS += \
./mtkernel_3/kernel/sysdepend/cpu/stm32h7/cpu_clock.d \
./mtkernel_3/kernel/sysdepend/cpu/stm32h7/vector_tbl.d 

mtkernel_3/kernel/sysdepend/cpu/stm32h7/%.o: ../kernel/sysdepend/cpu/stm32h7/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
