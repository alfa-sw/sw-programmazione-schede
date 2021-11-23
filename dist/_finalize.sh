cd `dirname $0`

mkdir out
cp _README.txt out/README.txt
cp *.whl out/
cp alfa_fw_upgrader_launcher out/alfa_fw_upgrader
cp alfa_fw_upgrader_launcher.exe out/alfa_fw_upgrader.exe
VER=$(cat __version__)

zip -r ../alfa_fw_upgrader_bdist_$VER.zip out/*

