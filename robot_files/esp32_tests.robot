*** Settings ***
Library    Process    #built-in library
Library    SerialLibrary    #external 3rd-party library
Library    BluetoothTesting.py    #custom library

Test Setup    Run Keywords
...           Open Serial Port

Test Teardown    Delete All Ports

*** Variables ***
${char_uuid}    ece27bad-3d4b-4072-8494-76a551f0b6cc        #BLE characteristic UUID
${ble_mac}    f0:08:d1:d5:0c:ae      #BLE device access address
${esp32_dev_path}    /dev/ttyUSB0    #dev path for serial communication

*** Test Cases ***

Test LED Switch On Via GPIO
    [Tags]    GPIO
    Turn LED On
    ${led_state_gpio}=    Get LED State
    Should Be Equal As Integers    ${led_state_gpio}    1

Test LED Switch Off Via GPIO
    [Tags]    GPIO
    Turn LED Off
    ${led_state_gpio}=    Get LED State
    Should Be Equal As Integers    ${led_state_gpio}    0

Test LED Switch On Via BLE
    [Tags]    BLE
    Turn LED On
    ${led_state_ble}=    Read Char Value    ${ble_mac}    ${char_uuid}
    Should Be Equal As Integers    ${led_state_ble}    1

Test LED Switch Off Via BLE
    [Tags]    BLE
    Turn LED Off
    ${led_state_ble}=    Read Char Value    ${ble_mac}    ${char_uuid}
    Should Be Equal As Integers    ${led_state_ble}    0

*** Keywords ***
Turn LED On
    Write Data    0x31    #0x31 is ascii '1'

Turn LED Off
    Write Data    0x30    #0x31 is ascii '0'

Get LED State
    ${result}=    Run Process    gpio    read    0
    Log    all output: ${result.stdout}
    [Return]    ${result.stdout}

Open Serial Port
    Add Port   ${esp32_dev_path}
    ...        baudrate=115200
    ...        bytesize=8
    ...        parity=N
    ...        stopbits=1
    ...        timeout=999
