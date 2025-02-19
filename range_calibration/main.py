import numpy as np
import serial
import time
import matplotlib.pyplot as plt
import pathlib


baudrate = 9600
port_detectors = "COM5"
STRING_LENGTH = 11
available_commands = ["g", "c", "b"]
cicle_count = 5
x = []
y = []
connection_detectors = serial.Serial(port_detectors, baudrate)


def send_command(cmd: str, response_len: int, connection: serial.Serial) -> tuple:
    connection.write(cmd.encode())

    if connection.in_waiting > 0:
        data = connection.read(response_len).decode('utf-8').rstrip()

        try:
            num1, num2 = map(int, data.split())

            return num1, num2
        except ValueError:
            print(f"Ошибка разбора чисел")

    return None, None


def save_value(x, y):
    with open("data.txt", "a") as file:
        file.write(f"{x} {y}\n")


file_index = len(list(pathlib.Path(".").glob("*.txt")))
file_name = f"data{file_index}.txt"
print(file_name)

while True:
    command = input("Введите команду (g - считать, c - цикл 100 значений): ")
    if command in available_commands:
        if command == "g":
            num1, num2 = send_command(command, STRING_LENGTH, connection_detectors)
            print(f"Число 1: {num1}, Число 2: {num2}")

        elif command == "c":
            plt.ion()
            x, y = [], []
            for i in range(cicle_count):
                num1, num2 = send_command("g", STRING_LENGTH, connection_detectors)
                if num1 is None or num2 is None:
                    time.sleep(0.1)
                    continue

                x.append(num1)
                y.append(num2)
                save_value(num1, num2)
                print(f"Число 1: {num1}, Число 2: {num2}")
                plt.scatter(x, y, c='b', s=15, marker='.')
                plt.pause(0.3)
            plt.ioff()

            x, y = np.array(x, dtype=int), np.array(y, dtype=int)
            plt.scatter(x, y, c='b', s=15, marker='.')
            plt.show()

            np.savetxt(file_name, np.stack((x, y)).T.astype(int))

            file_index = len(list(pathlib.Path(".").glob("*.txt")))
            file_name = f"data{file_index}.txt"
            print(file_name)

        elif command == "b":
            break
