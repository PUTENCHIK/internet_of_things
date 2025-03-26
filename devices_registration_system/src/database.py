from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker


def get_db_session():
    session = DBSession()
    try:
        yield session
    finally:
        session.close()


database_name = "./database.db"
database_path = f"sqlite:///{database_name}"

engine = create_engine(database_path, connect_args={
    "check_same_thread": False
})

DBSession = sessionmaker(
    autocommit=False,
    autoflush=False,
    bind=engine
)

BaseDBModel = declarative_base()