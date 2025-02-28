import time
import serial
import pathlib
import numpy as np
import matplotlib.pyplot as plt


def send_command(cmd: str, response_len: int, connection: serial.Serial) -> tuple:
    connection.write(cmd.encode())
    if connection.in_waiting > 0:
        data = connection.read(response_len).decode('utf-8').rstrip()
        try:
            num1, num2 = map(int, data.split())
            return num1, num2
        except ValueError:
            print(f"Данные: {data}; ожидаемый формат: [aaaa bbbb\\n]")
    return None, None


def get_data() -> tuple:
    num1, num2 = send_command("g", string_length, connection_detectors)
    print(f"Данные: {num1}, {num2}")
    return num1, num2


def show_data(x: list, y: list):
    x, y = np.array(x, dtype=int), np.array(y, dtype=int)
    z_start, z_end = 3, 7
    count = 1
    for z in range(z_start, z_end):
        coefs = np.polyfit(x, y, z)

        x_trend = np.linspace(x.min(), x.max(), x.shape[0], dtype=int)
        y_trend = np.zeros_like(x, dtype="float64")
        polynom = []
        for i, c in enumerate(coefs):
            y_trend += c * x_trend ** (z - i)
            match z-i:
                case 0: polynom += [f"{c:.3f}"]
                case 1: polynom += [f"{c:.3f}x"]
                case _: polynom += [f"{c:.3f}x^{z-i}"]

        plt.subplot(2, 2, count)
        count += 1
        plt.title(" + ".join(polynom))
        plt.plot(x_trend, y_trend, c='r')
        plt.scatter(x, y, s=3, c='b')

    plt.show()


def save_data(x: list, y: list):
    x, y = np.array(x), np.array(y)

    file_index = len(list(data_dir.glob("*.txt")))
    file_name = f"data{file_index}.txt"
    data_dir.mkdir(exist_ok=True)
    np.savetxt(str(data_dir / file_name), np.stack((x, y)).T.astype(int))
    print(f"Данные сохранены в data/{file_name}")


baudrate = 9600
port_detectors = "COM5"
string_length = 11
points_amount = 200
delay = 5
distance_max = 40
distance_min = 3
connection_detectors = serial.Serial(port_detectors, baudrate)

root = pathlib.Path(__file__).parent
data_dir = root / "data"

print(
f"""
    g - считать моментальные данные;
    c - считать {points_amount} точек с {delay}с задержкой;
    b - остановить программу.
"""
)

while True:
    command = input(f">> ")
    match command:
        case "g":
            num1, num2 = get_data()
            print(f"Данные: {num1}, {num2}")
        case "c":
            time.sleep(delay)
            plt.ion()
            x, y = [], []
            counter = 0

            while counter < points_amount:
                time.sleep(0.1)
                print(f"{counter}) ", end='')
                num1, num2 = get_data()
                if None in [num1, num2] or num1 > 1000:
                    print("\tskip")
                    continue
                if num1 > distance_max or num1 < distance_min:
                    print(f"\tСохранение данных только в пределах дистанции: [{distance_min}, {distance_max}]см")
                    continue

                x.append(num1)
                y.append(num2)

                counter += 1
                plt.scatter(num1, num2, c='b', s=15, marker='.')
                plt.pause(0.1)
            plt.ioff()

            show_data(x, y)
            save_data(x, y)
        case "b":
            print("Программа завершена")
            break
        case _:
            print(f"Неизвестная команда: {command}\n")
