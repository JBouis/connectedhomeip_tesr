# STM32 Getting Started Guide

The stm32 platform is a [Matter](https://github.com/project-chip/connectedhomeip)
platform that uses stm32 FreeRTOS SDK.

---

-   [stm32 Getting Started Guide](#stm32-getting-started-guide)
    -   [Supported Chips](#supported-chips)
    -   [Matter Example Applications](#matter-example-applications)
    -   [Building the Example Application](#building-the-example-application)
    -   [Commissioning](#commissioning)
        -   [BLE-Thread mode](#ble-thread-mode)
    -   [Factory](#factory)

---

## Supported Chips

The stm32 platform is supported on:

-   [stm32wb55-dk](http://www.stm32micro.com/en/goods/proinfo/36.html)

## Matter Example Applications

Sample Matter applications are provided for the stm32 platform. They can be used
to speed up development. You can find them in the samples with `/stm32` subfolder.

## Building the Example Application

-   [Setup Matter Environment](./BUILDING.md)

-   Setup STLINK tools
    -  
        ```
        sudo apt-get install stlink-tools
        ```
 
-   Setup Chip environment
    -   for stm32wb5-Dk:
        ```
        export stm32_BOARD=stm32wb5-Dk
        ```
  
        ```
-   Building the Application

    If the lighting example is to be built:

    ```
    ./scripts/build/build_examples.py --target stm32-$stm32_BOARD-lighting build
    ```

-   The output image files are stored in the subdirectories under `out`, the
    subdirectory name is the same as the argument specified after the option
    `--target` when build the examples.

-   After building the application, `ST-LINK` tool is used to flash it to the
    board, please refer to the

     sudo st-flash --flash=470k  write chip-stm32-lighting-example.bin 0x08000000 

## Commissioning

There is one commissioning modes supported by stm32 platform:

### BLE-Thread mode

1. Build and Flash
2. The example will run automatically after booting the stm32 board.
3. Restore factory settings using B1 button by pressing at least 10 seconds
4. Commissioning with
   [Chip-Tool](https://github.com/project-chip/connectedhomeip/tree/master/examples/chip-tool),for
   example:
    ```
    ./chip-tool pairing ble-thread <node_id> <hex:xxx> <pin_code> <discriminator>
    ```


## Factory

1. To build the example with the stm32 Factory Data Provider, exactly the same
   steps as above but add argument `-factory` when using the `build_examples.py`
   script. For example:
   `./scripts/build/build_examples.py --target stm32-$stm32_BOARD-lighting-factory build`
2. For more usage details, please refer to the
   [Factory Tool User Guide](https://github.com/stm32iot/Tools/blob/main/factory_tool/README.md)
