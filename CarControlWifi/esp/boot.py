# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import os, machine
#os.dupterm(None, 1) # disable REPL on UART(0)
import gc
#import webrepl
#webrepl.start()
gc.collect()

import network
import socket
import time
from robot import RobotController

ssid = "Glebiks"
password = "toster123"


try:
    robot = RobotController(0, baudrate=9600)
    print("Robot Controller loaded successfully")
except Exception as e:
    print("Error initializing RobotController:", e)


def connect_wifi():
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(ssid, password)
        while not sta_if.isconnected():
            pass
    print('network config:', sta_if.ifconfig())
    return sta_if.ifconfig()[0]


def webpage():
    html = """<!DOCTYPE html>
    <html>
    <head><title>Robot Control</title></head>
    <body>
    <h1>Robot Control</h1>
    <button onclick="location.href='/forward'">Forward</button><br><br>
    <button onclick="location.href='/backward'">Backward</button><br><br>
    <button onclick="location.href='/left'">Turn Left</button><br><br>
    <button onclick="location.href='/right'">Turn Right</button><br><br>
    <button onclick="location.href='/rotate_left'">Rotate Left</button><br><br>
    <button onclick="location.href='/rotate_right'">Rotate Right</button><br><br>
    <button onclick="location.href='/stop'">Stop</button><br><br>
    </body>
    </html>
    """
    return html


def main():
    ip_address = connect_wifi()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('', 80))
    s.listen(5)
    print('listening on', ip_address, ':80')

    while True:
        cl, addr = s.accept()
        request = cl.recv(1024)
        request = str(request)
        print('request123:')
        print(request)

        if "GET /forward" in request:
            print("forward")
            robot.forward()
        elif "GET /backward" in request:
            print("backward")
            robot.backward()
        elif "GET /left" in request:
            print("left")
            robot.turn_left()
        elif "GET /right" in request:
            print("right")
            robot.turn_right()
        elif "GET /rotate_left" in request:
            print("rotate left")
            robot.rotate_left()
        elif "GET /rotate_right" in request:
            print("rotate right")
            robot.rotate_right()
        elif "GET /stop" in request:
            print("stop")
            robot.stop()

        response = webpage()
        cl.send('HTTP/1.1 200 OK\n')
        cl.send('Content-type: text/html\n')
        cl.send('Connection: close\n\n')
        cl.sendall(response.encode())
        cl.close()

try:
    main()
except KeyboardInterrupt:
    print("Exiting...")
