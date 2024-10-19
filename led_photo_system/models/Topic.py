class Topic:
    unique_id_template = "UNIQUE_ID"
    common = f"led/{unique_id_template}/photo/"

    def __init__(self, name: str, is_track: bool):
        self.name = name
        self.is_track = is_track

    def set_unique_id(self, unique_id: str):
        self.name = self.name.replace(Topic.unique_id_template, unique_id)

    def __str__(self):
        return Topic.common + self.name