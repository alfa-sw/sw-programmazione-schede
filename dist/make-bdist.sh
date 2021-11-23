#!/bin/bash

VENV_DIR=~/work/sw/env_test3.9
RS485_DIR=~/work/sw/rs485-master
ALFAFWUP_DIR=~/work/sw/sw-programmazione-schede
OUT_DIR=~/work/sw/bdist

. $VENV_DIR/bin/activate

cd `dirname $0`
rm -R $OUT_DIR/stage1
rm -R $OUT_DIR/stage1_win
rm -R $OUT_DIR/stage2

mkdir $OUT_DIR/stage1
mkdir $OUT_DIR/stage2


cp _make_exe_win.bat $OUT_DIR/stage1/make_exe_win.bat
cp _make_exe_linux.sh $OUT_DIR/stage1/make_exe_linux.sh
cp _finalize.sh $OUT_DIR/stage2/finalize.sh
cp _README.txt _CHANGELOG.txt $OUT_DIR/stage2/

cd $RS485_DIR
rm dist/*.whl
python setup.py bdist_wheel
cp dist/*.whl $OUT_DIR/stage1/
cp dist/*.whl $OUT_DIR/stage2/
cp __version__ $OUT_DIR/stage2/

cd $ALFAFWUP_DIR
rm dist/*.whl
python setup.py bdist_wheel
cp dist/*.whl $OUT_DIR/stage1/
cp dist/*.whl $OUT_DIR/stage2/
cp -R src $OUT_DIR/stage1/alfa_fw_upgrader_src
cp bin/alfa_fw_upgrader_launcher.py $OUT_DIR/stage1/

cp -R $OUT_DIR/stage1 $OUT_DIR/stage1_win
mv $OUT_DIR/stage1_win/rs485* 
