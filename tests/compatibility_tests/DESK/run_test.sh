#!/bin/bash

PROGRAMMER_SNO="BUR210573935"

flash_boot () {
    NAME=$1
    MICRO=$2
    FILE=$3

    ARG=""
    if [[ "$FILE" == "" ]]; then
        echo "erasing $NAME $FILE"
    else
        ARG="-M -F$FILE"
        echo "flashing $NAME $FILE to $MICRO"
    fi
    
    read -p "Press [Enter] key to continue..."
    
    (set -x; /opt/microchip/mplabx/v5.50/sys/java/zulu8.40.0.25-ca-fx-jre8.0.222-linux_x64/bin/java \
      -jar /opt/microchip/mplabx/v5.50/mplab_platform/mplab_ipe/ipecmd.jar \
      -$MICRO -L -E -TS$PROGRAMMER_SNO $ARG)
    if [ ! $? -eq 0 ]; then
        echo "failed to flash"
        exit 1
    fi
}

flash_app () {
    ID=$1
    FILE=$2
    echo flashing $FILE, device=$ID
    #read -p "Press [Enter] key to continue..."
    (set -x; alfa_fw_upgrader -p /dev/ttyUSB1 -cr 5 -s serial -d $ID -f $FILE info program verify jump)
    if [ ! $? -eq 0 ]; then
        echo "failed to update"
        exit 1
    fi
}

get_info () {
    ID=$1
    FILE=$2
    echo flashing $FILE, device=$ID
    #read -p "Press [Enter] key to continue..."
    (set -x; alfa_fw_upgrader -p /dev/ttyUSB1 -cr 5 -s serial -d 255 info jump)
    if [ ! $? -eq 0 ]; then
        echo "failed to update"
        exit 1
    fi
}

flash_boot_test_1_2_3_4() {
    # N°1
    #	FW BOOT	FW APPLICATIVO
    # MAB	≥ 1.0.2	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	≥ 1.5.2	≥ 4.2.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_boot "MAB" "P24FJ256GB106" "SchedaMAB/BootLoader/bootloaderMABrd_1.0.2.hex"

    # SLAVE_1
    flash_boot "SLAVE_1" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # SLAVE_8
    flash_boot "SLAVE_8" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # CAN_LIFTER
    flash_boot "CAN_LIFTER" "P33FJ32MC204" "SchedaSGABELLO/BOOT/BootLoaderCANLIFTER_1.6.2.hex"

    # HUMIDIFIER
    flash_boot "HUMIDIFIER" "P24FJ64GA704" "SchedaUMIDIFICATORE/BOOT/BootLoaderUMIDIFICATORE_1.7.2.hex"
}

flash_app_test_1_5_6_7_8() {
    # N°1
    #	FW BOOT	FW APPLICATIVO
    # MAB	≥ 1.0.2	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	≥ 1.5.2	≥ 4.2.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_app "255" "SchedaMAB/Applicativo/desk_3.2.5-boot-Slave_1_8_42_43.hex"

    # SLAVE_8
    flash_app "8" "SchedaSCCB/Applicativi/pump-r1-siboot-dipswitch_4_2_0.hex"


    # SLAVE_1
    flash_app "1" "SchedaSCCB/Applicativi/pump-r1-siboot-dipswitch_4_2_0.hex"

    # CAN_LIFTER (42)
    flash_app "42" "SchedaSGABELLO/APPLICATIVI/can_lifter-boot-dipswitch_3.2.0.hex"

    # HUMIDIFIER (43)
    flash_app "43" "SchedaUMIDIFICATORE/APPLICATIVO/umidificatore-r1-siboot-dipswitch_3_2_0.hex"
}

flash_app_test_2() {
    # N°2
    #	FW BOOT	FW APPLICATIVO
    # MAB	≥ 1.0.2	< 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	≥ 1.5.2	≥ 4.2.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_app "255" "SchedaMAB/Applicativo/desk_3.2.2-boot.hex"

    # SLAVE_1
    flash_app "1" "SchedaSCCB/Applicativi/pump-r1-siboot-dipswitch_4_2_0.hex"

    # SLAVE_8
    flash_app "8" "SchedaSCCB/Applicativi/pump-r1-siboot-dipswitch_4_2_0.hex"

    # CAN_LIFTER (42)
    flash_app "42" "SchedaSGABELLO/APPLICATIVI/can_lifter-boot-dipswitch_3.2.0.hex"

    # HUMIDIFIER (43)
    flash_app "43" "SchedaUMIDIFICATORE/APPLICATIVO/umidificatore-r1-siboot-dipswitch_3_2_0.hex"
}

flash_app_test_3() {
    # N°3
    #	FW BOOT	FW APPLICATIVO
    # MAB	≥ 1.0.2	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	≥ 1.5.2	= 4.1.4
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_app "255" "SchedaMAB/Applicativo/desk_3.2.5-boot-Slave_1_8_42_43.hex"

    # SLAVE_1
    flash_app "1" "SchedaSCCB/Applicativi/pump-r1-siboot-dipswitch_4_2_0.hex"

    # SLAVE_8
    flash_app "8" "SchedaSCCB/Applicativi/pump-r1-siboot-dipswitch_4_1_4.hex"

    # CAN_LIFTER (42)
    flash_app "42" "SchedaSGABELLO/APPLICATIVI/can_lifter-boot-dipswitch_3.2.0.hex"

    # HUMIDIFIER (43)
    flash_app "43" "SchedaUMIDIFICATORE/APPLICATIVO/umidificatore-r1-siboot-dipswitch_3_2_0.hex"
}

flash_app_test_4() {
    # N°4
    #	FW BOOT	FW APPLICATIVO
    # MAB	≥ 1.0.2	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	≥ 1.5.2	= 3.0.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_app "255" "SchedaMAB/Applicativo/desk_3.2.5-boot-Slave_1_8_42_43.hex"

    # SLAVE_1
    flash_app "1" "SchedaSCCB/Applicativi/pump-r1-siboot-dipswitch_4_2_0.hex"

    # SLAVE_8
    flash_app "8" "SchedaSCCB/Applicativi/color-300-r1-siboot-dipswitch_3_0_0.hex"

    # CAN_LIFTER (42)
    flash_app "42" "SchedaSGABELLO/APPLICATIVI/can_lifter-boot-dipswitch_3.2.0.hex"

    # HUMIDIFIER (43)
    flash_app "43" "SchedaUMIDIFICATORE/APPLICATIVO/umidificatore-r1-siboot-dipswitch_3_2_0.hex"
}

flash_boot_test_5() {
    # N°5
    #	FW BOOT	FW APPLICATIVO
    # MAB	< 1.0.2	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	≥ 1.5.2	≥ 4.2.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_boot "MAB" "P24FJ256GB106" "SchedaMAB/BootLoader/bootloaderMABrd_1.0.1.hex"

    # SLAVE_1
    flash_boot "SLAVE_1" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # SLAVE_8
    flash_boot "SLAVE_8" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # CAN_LIFTER
    flash_boot "CAN_LIFTER" "P33FJ32MC204" "SchedaSGABELLO/BOOT/BootLoaderCANLIFTER_1.6.2.hex"

    # HUMIDIFIER
    flash_boot "HUMIDIFIER" "P24FJ64GA704" "SchedaUMIDIFICATORE/BOOT/BootLoaderUMIDIFICATORE_1.7.2.hex"
}


flash_boot_test_6() {
    # N°6
    #	FW BOOT	FW APPLICATIVO
    # MAB	≥ 1.0.2	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	< 1.5.2	≥ 4.2.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_boot "MAB" "P24FJ256GB106" "SchedaMAB/BootLoader/bootloaderMABrd_1.0.2.hex"

    # SLAVE_1
    flash_boot "SLAVE_1" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # SLAVE_8
    flash_boot "SLAVE_8" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.1.hex"

    # CAN_LIFTER
    flash_boot "CAN_LIFTER" "P33FJ32MC204" "SchedaSGABELLO/BOOT/BootLoaderCANLIFTER_1.6.2.hex"

    # HUMIDIFIER
    flash_boot "HUMIDIFIER" "P24FJ64GA704" "SchedaUMIDIFICATORE/BOOT/BootLoaderUMIDIFICATORE_1.7.2.hex"
}


flash_boot_test_7() {
    # N°7
    #	FW BOOT	FW APPLICATIVO
    # MAB	ASSENTE	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	≥ 1.5.2	≥ 4.2.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB ASSENTE
    flash_boot "MAB" "P24FJ256GB106" ""

    # SLAVE_1
    flash_boot "SLAVE_1" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # SLAVE_8
    flash_boot "SLAVE_8" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # CAN_LIFTER
    flash_boot "CAN_LIFTER" "P33FJ32MC204" "SchedaSGABELLO/BOOT/BootLoaderCANLIFTER_1.6.2.hex"

    # HUMIDIFIER
    flash_boot "HUMIDIFIER" "P24FJ64GA704" "SchedaUMIDIFICATORE/BOOT/BootLoaderUMIDIFICATORE_1.7.2.hex"
}


flash_boot_test_8() {
    # N°8
    #	FW BOOT	FW APPLICATIVO
    # MAB	≥ 1.0.2	≥ 3.2.5
    # SLAVE_1	≥ 1.5.2	≥ 4.2.0
    # SLAVE_8	ASSENTE	≥ 4.2.0
    # CAN_LIFTER	≥ 1.6.2	≥ 3.2.0
    # HUMIDIFIER	≥ 1.7.2	≥ 3.2.0

    # MAB
    flash_boot "MAB" "P24FJ256GB106" "SchedaMAB/BootLoader/bootloaderMABrd_1.0.2.hex"

    # SLAVE_1
    flash_boot "SLAVE_1" "P33FJ32MC204" "SchedaSCCB/Bootloader/BootLoaderSCCB_1.5.2.hex"

    # SLAVE_8 ASSENTE
    flash_boot "SLAVE_8" "P33FJ32MC204" ""

    # CAN_LIFTER
    flash_boot "CAN_LIFTER" "P33FJ32MC204" "SchedaSGABELLO/BOOT/BootLoaderCANLIFTER_1.6.2.hex"

    # HUMIDIFIER
    flash_boot "HUMIDIFIER" "P24FJ64GA704" "SchedaUMIDIFICATORE/BOOT/BootLoaderUMIDIFICATORE_1.7.2.hex"
}


if [[ $# -le 1 ]]; then
    echo "Usage: script.sh TEST_ID boot|app"
    exit 2
fi

TEST_ID=$1
TYPE=$2

cd "$(dirname "$0")"

case $TEST_ID in
1)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_1_2_3_4
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_1_5_6_7_8
    elif [[ "$TYPE" == "info" ]]; then
       get_info
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;
2)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_1_2_3_4
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_2
    elif [[ "$TYPE" == "info" ]]; then
       get_info
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;
3)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_1_2_3_4
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_3
    elif [[ "$TYPE" == "info" ]]; then
       get_info
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;
4)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_1_2_3_4
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_4
    elif [[ "$TYPE" == "info" ]]; then
       get_info       
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;
5)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_5
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_1_5_6_7_8
    elif [[ "$TYPE" == "info" ]]; then
       get_info       
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;
6)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_6
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_1_5_6_7_8
    elif [[ "$TYPE" == "info" ]]; then
       get_info       
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;    
7)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_7
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_1_5_6_7_8
    elif [[ "$TYPE" == "info" ]]; then
       get_info       
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;
8)
    if [[ "$TYPE" == "boot" ]]; then
       flash_boot_test_8
    elif [[ "$TYPE" == "app" ]]; then
       flash_app_test_1_5_6_7_8
    elif [[ "$TYPE" == "info" ]]; then
       get_info       
    else
       echo "Invalid selection"
       exit 2
    fi
    ;;    
*)
    echo "Invalid selection"
    exit 2
    ;;
esac
