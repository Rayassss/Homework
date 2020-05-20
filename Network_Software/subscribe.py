#!/usr/bin/env python3  
#coding=utf-8  
import json
import sys
import os
import paho.mqtt.client as mqtt
import time
import random
sys.path.append(os.path.abspath(os.path.dirname(__file__) + '/' + '..'))
sys.path.append("..")

def on_connect(client, userdata, flags, rc):
	print('Connected to MQTT with resurt code ', rc)
	print('MQTT Server IPv4:', HOST_SUB)
	client.subscribe(REPORT_TOPIC)
	print('Subscribe Topic:', REPORT_TOPIC)

def on_message(client, userdata, msg):
    payload = str(msg.payload.decode('utf-8'))
    print(payload)

def server_conenet(client):
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(HOST_SUB, 1883, 60)
    client.loop_start()

def server_stop(client):
    client.loop_stop()
    sys.exit(0)

def subscribe_mqtt(topic,host):
    global REPORT_TOPIC
    global HOST_SUB
    REPORT_TOPIC = topic
    HOST_SUB = host
    client_id = time.strftime('%Y%m%d%H%M%S', time.localtime(time.time()))
    client = mqtt.Client(client_id, transport='tcp')
    server_conenet(client)


