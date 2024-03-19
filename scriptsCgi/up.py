#!/usr/bin/python3

import sys
import os
import uuid
import time
import signal

def timeout_handler(signum, frame):
    if os.path.exists(filename):
        os.remove(filename)

signal.signal(signal.SIGALRM, timeout_handler)


print("Content-Type: text/html\r")
print("Status: 201 created\r")
print("\r\n")

signal.alarm(6)
upload_dir = './cgi-bin'
if not os.path.exists(upload_dir):
    os.makedirs(upload_dir)

random = str(uuid.uuid4())[:8]

filename = os.path.join(upload_dir, random + '_uploaded_file' + os.environ.get('EXTENSION_'))

chunk_size = 2048
with open(filename, 'wb') as file:
    while True:
        data_chunk = sys.stdin.buffer.read(chunk_size)
        if not data_chunk:
            break
        file.write(data_chunk)
signal.alarm(0)
    