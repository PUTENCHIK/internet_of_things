import threading
from typing import Optional
from fastapi import FastAPI, BackgroundTasks, Depends

from src.database import get_db_session, BaseDBModel, engine
from src.schemes import DeviceRegistration, Device
from src.bot import send_telegram_notification, start_bot
from src.crud import _register_device


app = FastAPI()
BaseDBModel.metadata.create_all(bind=engine)
threading.Thread(target=start_bot, daemon=True).start()


@app.post("/register/", response_model=Optional[Device])
def register_device(device: DeviceRegistration,
                    tasks: BackgroundTasks,
                    db = Depends(get_db_session)):
    
    new_device = _register_device(device, db)
    
    if new_device is not None:
        message = (
            f"New device registered!\n"
            f"ID: {new_device.id}\n"
            f"MAC: {new_device.device_mac}"
        )
    else:
        message = "New device is not registered"
        
    tasks.add_task(send_telegram_notification, message)
    return new_device
