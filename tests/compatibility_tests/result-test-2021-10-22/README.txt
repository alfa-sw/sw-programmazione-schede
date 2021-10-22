GOAL OF THE TEST SESSION
========================

This test session has been performed with the goal to check whether the system
with alfa boards (MMT + various SCCB) and the update application works correctly
in different scenarios.

HOW THE TESTS HAVE BEEN DONE
============================

The test specifications can be found in the document Debug_Aggiornamento_FW_REV01.docx,
with the following differences:

- starting condition: for each test (scenario), the entire flash is erased and
  reprogrammed;
- another test is done (test_1_repeat_jumps)

The procedure is performed by the script: tests/compatibility_tests/test.py
Although the script can execute the whole bunch of tests, I recommend to execute
a single test per execution, because the procedure may fail:
 - Micro programming by using PICKIT is not guaranteed to work correctly - they
   can really drive you crazy, you have to plug in&out the USB, retry etc.;
 - The procedure is semi-automatic, an human mistake is possible and it is better
   to repeat a single test than the whole series;
 - The update procedure is not 100% affordable to now, somethings goes wrong
   sometimes.
For these reasons, you should disable all the tests except the one you want to
execute, by using the statement "@unittest.SkipTest" in the code of test.py.
 
In order to execute the test, prepare as follows:
 - setup 1 MMT, and 2 SCCB; set the dipswitch to 1-0-0-0-0-0 for the slave
   board with address 1, and to 0-0-0-1-0-0 for the slave with address 8
 - connect 2 PICKIT via USB and prepare to attach them to the boards
 
The first tests requires to have only MMT and SCCB with address 8.

Once setup the environment, select the test editing test.py and execute:

 python test.py 2>&1 | tee your_log.log

Follow the instructions on the prompt and keep fingers crossed :-)

DESCRIPTION OF THE TESTS
========================

 - test_1_repeat_jumps: program bootloader and application with files specified
   in scenario 1 (working scenario), then, for 10 times:
   1. retrieve info for serial commands
   2. jump to boot
   3. retrieve other data from USB commands
   4. jump to application
   
   These sequence is performed by the command:
    alfa_fw_upgrader  -vv -s serial info jump
    
- test_scenario_1..10: program bootloader and application as specified in the doc

NOTE: the test 5 is repeated twice in the doc, so we have test_scenario_5 and
test_scenario_5bis.

ATTACHMENTS
===========

For further reference I have included hex files used in hex.zip and the log
of each test in logs.zip. Beware that the some files contains only the
standard output, not stderr.


RESULTS OF THE TESTS
====================

IMPORTANT NOTE: a test is marked OK if at least one attempt is successful.
Some of the tests marked as OK have been repeated due to fw/bootloader/communication
issues.

* test_1_repeat_jumps: OK
* test_scenario_1:     OK
* test_scenario_2:     OK
* test_scenario_3:     PARTIAL FAIL - SCCB has been updated too [note 1]
* test_scenario_4:     OK
* test_scenario_5:     OK
* test_scenario_6:     PARTIAL FAIL - SCCB/1 has been updated too
* test_scenario_7:     OK
* test_scenario_8:     OK
* test_scenario_9:     OK
* test_scenario_10:    OK [note 2]

[note 1]: 1 attempt failed to go to boot mode - see log test_out_scenario3_fallito.log
[note 2]: 1 attempt failed because of failed USB initialization - see log
          test_out_scenario10_fallito.log


CONCLUSIONS
===========

Many problems related to bootloader encountered while performing these tests have been addressed.
It seems that the problem causing the system to hang until power reset has been fixed (issues #13). 
However, the procedure itself is not completely affordable, that is the piece of software
that will handle the upgrade of the system will have to handle failures.
The most common causes of failures are:
 1. issue #12 - MMT fails to go to bootloader/update mode (despite of changes in upgrade tool, 
    see commit d439d4f) -> review of bootloader codebase and/or review of logic to go in boot mode
    in upgrade tool;
 2. failure in verifying the program;
 3. failure in USB initialization.
 
 
