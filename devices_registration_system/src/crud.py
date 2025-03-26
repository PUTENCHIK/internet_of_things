from typing import Optional
from sqlalchemy.orm import Session

from src.schemes import DeviceRegistration
from src.models import Device as DeviceModel


def _register_device(device: DeviceRegistration, db: Session) -> Optional[DeviceModel]:
    new_device = DeviceModel(
        device_mac=device.device_mac
    )
    
    db.add(new_device)
    db.commit()
    db.refresh(new_device)
    
    return new_device
