cd `dirname $0`

python3.9 -m venv venv
. venv/bin/activate
pip install pyinstaller
pip install rs485_master-*.whl
pip install alfa_fw_upgrader-*.whl
pyinstaller --add-data "./alfa_fw_upgrader_src/alfa_fw_upgrader:alfa_fw_upgrader" --copy-metadata alfa_fw_upgrader --onefile alfa_fw_upgrader_launcher.py

