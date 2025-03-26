from pydantic import BaseModel


class DeviceRegistration(BaseModel):
    device_mac: str


class Device(BaseModel):
    id: int
    device_mac: str
