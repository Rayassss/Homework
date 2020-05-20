wget https://dl.espressif.com/dl/xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
tar -zxvf xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
sudo mv ./xtensa-lx106-elf /opt/
echo "PATH=$PATH:/opt/xtensa-lx106-elf/bin" >> ~/.zshrc
echo "PATH=$PATH:/opt/xtensa-lx106-elf/bin" >> ~/.bashrc
sudo apt-get install python3 python3-pip git lib32ncurses5 lib32z1 gperf bison flex
export alias python='/usr/bin/python3'
sudo ln -s /usr/bin/python3.5 /usr/bin/python
git clone https://github.com/espressif/ESP8266_RTOS_SDK.git
cd ESP8266_RTOS_SDK
pip3 install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple
pip3 install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple
echo "IDF_PATH=$(dirname $(readlink -f $0))" >> ~/.bashrc
echo "IDF_PATH=$(dirname $(readlink -f $0))" >> ~/.zshrc

