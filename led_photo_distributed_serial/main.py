import serial

responses = {'d': 7,
             'u': 6,
             'p': 1,}
LIMIT_1023 = 500
LIMIT_255 = LIMIT_1023 // 4

port_led = "COM4"               # порт микроконтроллера с светодиодом
port_photo = "COM5"             # порт микроконтроллера с фоторезистором

connection_led = serial.Serial(port_led, timeout=1)
connection_photo = serial.Serial(port_photo, timeout=1)


def send_command(cmd: str, response_len: int, connection: serial.Serial) -> int:
    str_resp: int = 0
    connection.write(cmd.encode())
    if response_len > 0:
        resp: bytes = connection.read(response_len)
        str_resp = int.from_bytes(resp, 'little')

    return str_resp


while True:
    # получение значения фоторезистора с помощью отправки соответствующей команды на микроконтроллер
    photo_val_resp: int = send_command('p', responses['p'], connection_photo)

    # определение команды на включение/выключение светодиода в зависимости от значения фоторезистора
    led_command = 'u' if photo_val_resp > LIMIT_255 else 'd'
    # отправка команды
    send_command(led_command, responses[led_command], connection_led)
