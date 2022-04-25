################################################################################
# micro T-Kernel 3.0 BSP  makefile
################################################################################

OBJS += \
./mtkernel_3/lib/libtm/sysdepend/nucleo_h723/tm_com.o 

C_DEPS += \
./mtkernel_3/lib/libtm/sysdepend/nucleo_h723/tm_com.d 

mtkernel_3/lib/libtm/sysdepend/nucleo_h723/%.o: ../lib/libtm/sysdepend/nucleo_h723/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


