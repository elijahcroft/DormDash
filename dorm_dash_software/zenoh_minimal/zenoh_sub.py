import zenoh, time
from zenoh import Config
def listener(sample):
    print(f"{sample.key_expr} => {sample.payload.to_string()}")

with zenoh.open(Config()) as session:
    with session.declare_subscriber('demo/example/**', listener) as subscriber:
        time.sleep(60)