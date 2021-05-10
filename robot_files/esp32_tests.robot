*** Settings ***
Library    Process
Library    SerialLibrary
Library    BluetoothTesting.py

Test Setup    Run Keywords
...           Open Serial Port

Test Teardown    Delete All Ports

*** Test Cases ***

Check LED Can Turn Off
    Turn LED Off
    ${led_state_gpio}=    Get LED State
    Should Be Equal As Integers    ${led_state_gpio}    0
    ${led_state_ble}=    Read Char Value
    Should Be Equal As Integers    ${led_state_ble}    0

Check LED Can Turn On
    Turn LED On
    ${led_state_gpio}=    Get LED State
    Should Be Equal As Integers    ${led_state_gpio}    1
    ${led_state_ble}=    Read Char Value
    Should Be Equal As Integers    ${led_state_ble}    1

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
    Add Port   /dev/ttyUSB0
    ...        baudrate=115200
    ...        bytesize=8
    ...        parity=N
    ...        stopbits=1
    ...        timeout=999