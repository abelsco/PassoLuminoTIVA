################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
driverlib/%.obj: ../driverlib/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/abelsco/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/abelsco/Google Drive/8 Periodo/SISTEMAS EMBARCADOS/aula19_10/Motor_passo_angulo" --include_path="/home/abelsco/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C1294NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

driverlib/%.obj: ../driverlib/%.s $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/abelsco/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/abelsco/Google Drive/8 Periodo/SISTEMAS EMBARCADOS/aula19_10/Motor_passo_angulo" --include_path="/home/abelsco/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C1294NCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="driverlib/$(basename $(<F)).d_raw" --obj_directory="driverlib" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


