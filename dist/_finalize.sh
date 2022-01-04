cd `dirname $0`

mkdir out
cp _README.txt out/README.txt
cp _CHANGELOG.txt out/CHANGELOG.txt
cp *.whl out/
cp alfa_fw_upgrader_gui out/alfa_fw_upgrader_gui
cp alfa_fw_upgrader_gui.exe out/alfa_fw_upgrader_gui.exe
VER=$(cat __version__)

cd out
zip -r ../../alfa_fw_upgrader_bdist_$VER.zip *

