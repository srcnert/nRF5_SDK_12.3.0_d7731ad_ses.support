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
