set PYTHON_BIN=%USERPROFILE%\AppData\Local\Programs\Python\Python39\python.exe
%PYTHON_BIN% -m venv venv
CALL venv\Scripts\activate
pip install pyinstaller
for %%i in (rs485*.whl) do pip install %%i
for %%i in (alfa_fw_upgrader*.whl) do pip install %%i
pyinstaller --add-data "alfa_fw_upgrader_src\alfa_fw_upgrader;alfa_fw_upgrader" --add-binary "libusb-1.0.dll;." --copy-metadata alfa_fw_upgrader --onefile alfa_fw_upgrader_launcher.py




