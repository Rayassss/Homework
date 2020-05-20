#!/usr/bin/env python3  
#coding=utf-8
import subscribe
import time
HOST = "47.93.184.19"
if __name__ == '__main__':
    subscribe.subscribe_mqtt('humi',HOST)
    while True:
        time.sleep(1000)
