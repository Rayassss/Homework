@echo off
echo make sure that you have installed python3 and pip
pip install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install wheel -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install Pyinstaller -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install -i https://pypi.tuna.tsinghua.edu.cn/simple pyinstaller paho-mqtt
pyinstaller -F .\main.py -i %~dp0Icon.ico
copy %~dp0dist\main.exe %~dp0
rd/s/q %~dp0dist 
rd/s/q %~dp0build
rd/s/q %~dp0__pycache__
del %~dp0main.spec
echo Build Success!!!
pause


