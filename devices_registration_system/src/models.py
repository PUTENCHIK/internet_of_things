from sqlalchemy import Column, Integer, String
from src.database import BaseDBModel


class Device(BaseDBModel):
    __tablename__ = "devices"
    
    id = Column(Integer, primary_key=True, autoincrement=True)
    device_mac = Column(String, unique=True)
