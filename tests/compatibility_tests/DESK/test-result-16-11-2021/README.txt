GOAL OF THE TEST SESSION
========================

This test session has been performed with the goal to check whether a DESK system
with alfa boards (MAB + 2 SCCB + humidifier + can lifter) and the update application
works correctly in different scenarios.

TEST SETUP
==========

List of boards:
 - MAB
 - SCCB, address 1
 - SCCB, address 8
 - CAN LIFTER, address 42
 - HUMIDIFIER, address 43


In order to perform the tests the bash script run_test.sh has been developed.

To run the test $1, execute:

./run_test.sh $1 boot 2>&1 | tee log_test$1_boot.log
read -p "Press any key to resume ..."
./run_test.sh $1 app 2>&1 | tee log_test$1_app.log
./run_test.sh $1 info 2>&1 | tee log_test$1_info.log

ATTACHMENTS
===========

For each test defined in Debug_Aggiornamento_FW_REV02.docx, there 3 files:
 - log_test[TEST INDEX]_boot.log: log file of command to erase and flash bootloaders
 - log_test[TEST INDEX]_app.log: log file of commands to update boards application 
 - log_test[TEST INDEX]_info.log: log file of command to get info about versions
   after updating applications

File input_hex.zip contains hex files used in this test.

RESULTS SUMMARY
===============
 
 Test 1: OK
 Test 2: OK
 Test 3: OK
 Test 4: OK
         NOTE: SLAVE_8 provides FW version (43, 43, 0)
 Test 5: OK
 Test 6: OK
 Test 7: OK
 Test 8: OK
 
