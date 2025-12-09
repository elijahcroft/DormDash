import zenoh
from zenoh import Config

config = Config()

with zenoh.open(config) as session:
    session.put('demo/example/hello', 'Hello World!')


