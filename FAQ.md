# Frequently Asked Questions - FAQs

### Is the C5 already in mass production? 
- C5 is already in production. We encourage you to start a design on C5. 

### How will factory programming with OEMiRoT work?
- Factory programming - the question is not detailed enough to be able to answer. Factory programming is not different with or without OEMiROT, only possible question could be about secure programming and for this we don't provide SFI on C5. For mass market, maybe the question is related to the fact that STiROT is only provided with IAR ? this should be solved in June's delivery.
 
### Is there a static analisys tool integrated ? For MISRA requirement and safety certification.
- No, there is no plan to integrate such a tools inside CubeMX2.

### Will it support Zephyr development?
- STM32C5 is supported by Zephyr now.

### Any plans for supporting USB Host on Zephyr for the C5?
- We don't have plans yet for USB Host. Maybe foreseen in distant future (next year??).
 
### Why not to have fully C++ drivers?
- In embded coding especilay drivers the state of the art is to use c , not c++ this is to maximize the real time perfo and reducte the footprint.
 
### I have some feedback for the OEMiRoT example, including a cmake config. Where is the best place to share that?
- For feedback on OEMiROT, use either [STcommunity](https://community.st.com/) or [OLS](https://ols.st.com/s/)

### Is MX2 supporting old MCUs?
- No, at the moment we focus to support only new STM32 series available or comming soon (STM32C5, STM32V8). In the distant future we may add also some older MCUs probably but this is only a speculation now.

### Where can I find the STM32 Self-Test Library (STL) for the STM32C5?
- X-Cube-STL package is under development and it will be available soon.

### C5 seems to be a good candidate, but we have still concerns regarding the part availabality

### FreeRTOS in HALv2 is using the CMSIS-RTOS layer or the direct FreeRTOS API calls ?
- In HAL2 we are using direct FreeRTOS API calls

### Why FreeRTOS over ThreadX? Is this technical reason or commercial?
- More commercial reason. Seeing FreeRTOS is quite more popular.

### Is there any official Docker image with this setup already prepared to build our firmware? In my case, I use Docker to keep the build process reproducible, but I debug on my host machine because I’ve had issues accessing ports on macOS.
- No official Docker image is available from STMicroelectronics for STM32 firmware builds.

### What is EEPROM emulation? And what is for? Which real application it can be used?
- Internal NOR flash behaves differently than EEPROM (EEPROM allows more granular handling). For data storage (read and write of small data chunks) it is better to use EEPROM. If there is no internal EEPROM, then EEPROM emulation is an option. See this [AN4894](https://www.st.com/resource/en/application_note/an4894-how-to-use-eeprom-emulation-on-stm32-mcus-stmicroelectronics.pdf) and this [article](https://community.st.com/t5/stm32-mcus/how-to-use-eeprom-emulation-with-stm32cubemx2/ta-p/873139).

### Will the old USB stack be supported in coming versions of HAL2 or is USBX the stack of choice for HAL2?
- USBX is the choice, see [here](https://dev.st.com/stm32cube-docs/embedded-software/2.0.0/en/products.html#middleware)

### STM32C5 is a Cortex-M33. Wich is the main difference compared to STM32H5?
- Very briefly -> C5: 144 MHz, no TrustZone, simple clock system. H5: 250 MHz, with TrustZone, complex clock system. But there are more differences.

### Temperature Range?
- The devices operates in the –40 to +125 °C (+140 °C junction) temperature ranges and from a 2.7 to 3.6 V power supply.

### Release plan of HAL2 and CubeMX2?
- Already released.

### What precision FPU is used in STM32C5?
- Single precission.

### Where are the fabs for the STM32C5 - Is it in Europe? Also the packaging? 
- France. Yes (LQFP packages in the case of STM32C5).

### Are we allowed to share the presentation with colleagues?
- No confidential information shared, so yes.