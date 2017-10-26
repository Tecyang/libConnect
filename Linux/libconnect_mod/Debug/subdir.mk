################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mod_libconnect_mod.c \
../mod_pcap.c 

OBJS += \
./mod_libconnect_mod.o \
./mod_pcap.o 

C_DEPS += \
./mod_libconnect_mod.d \
./mod_pcap.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I/usr/include;usr/local/include;usr/local/apache2/include; -I/usr/include/c++/5.4.0 -I/usr/local/apache2/include -I/usr/include/apr-1.0 -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


