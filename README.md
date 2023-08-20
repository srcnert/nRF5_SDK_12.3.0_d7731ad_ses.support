# Segger Embedded Studio support for NRF5_SDK_12_3_0_d7732ad

- Ses projects added to examples_segger folder.
- Project in "/examples_segger/ble_peripheral/ble_app_template/pca10028/s130/ses_official"
directly created at SES (without import keil project).

# What has changed?

- Inside "hardfault_handler_gcc.c" file,

// "   ldr   r1, =__StackTop                  \n"

// "   ldr   r2, =__StackLimit                \n"

"   ldr   r1, =__stack_end__                \n"

"   ldr   r2, =__stack_start__              \n"


Following lines changed as above according to "https://devzone.nordicsemi.com/f/nordic-q-a/23232/hardfault_handler-for-segger-embedded-studio".


# Useful links and references

### Adafruit
- `undefined reference to `vtable for Adafruit_GFX`
I used the outdated library from a forum [here](https://forum.pjrc.com/threads/10-Adafruit-PCD8544-LCD-library). The latest library
was causing the link error above.
- To be sure the display was working, I connected it to an Arduino Nano, but my Ubuntu would not detect the Nano.
This [link](https://forum.arduino.cc/t/ubuntu-arduino-ide-not-showing-any-ports/1043925) had the solution.

### Using C++
- [Something about constructors](https://devzone.nordicsemi.com/f/nordic-q-a/42453/undefined-reference-to-vtable-for-__cxxabiv1-__si_class_type_info)
- [You should define virtual functions](https://gcc.gnu.org/faq.html#vtables). See the heading "When building C++, the linker says my constructors, destructors or virtual tables are undefined, but I defined them"
- You might be looking for a function [defined in a C++](https://gcc.gnu.org/legacy-ml/gcc-help/2009-10/msg00320.html) library `libsupc++`
- Helpful [link](https://isocpp.org/wiki/faq/mixing-c-and-cpp) when mixing C++ and C.

### SDK, IDE, Nordic, and other specific nuances
- The first argument to the SVCALL is meant to be uint16_t, but it was defined as uint8_t. Although already implemented in the SDK,
this [link](https://devzone.nordicsemi.com/f/nordic-q-a/13019/compiling-with-g-error-asm-operand-0-probably-doesn-t-match-constraints) has more information about why it was a problem. It was a known [bug](https://devzone.nordicsemi.com/f/nordic-q-a/2232/upgrading-to-sdk-5-2-0-breaks-build)
- Define a global macro in SES [here](https://studio.segger.com/index.htm?https://studio.segger.com/ide_project_macros.htm)
