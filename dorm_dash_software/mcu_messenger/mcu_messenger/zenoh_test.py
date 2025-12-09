import sys
from zenoh_controller import ZenohController, ZenohPublisher, Reliability

def create_test_subscriber(controller: ZenohController):
    
    controller.open_session()

    def test_callback(payload):
        print(f"Temperature: {payload}")

    controller.create_subscriber(
        key="robot/sensor/temperature",
        callback=test_callback  
    )

    try:
        while True:
            pass
    except KeyboardInterrupt:
        sys.exit(0)

def create_test_publisher(controller: ZenohController):
    
    controller.open_session()

    publisher: ZenohPublisher = controller.create_publisher(
        key="robot/sensor/temperature", 
        reliability=Reliability.RELIABLE
    )

    MAX_COUNT = 1500
    count: int = 0

    try: 
        while True:
            
            # publisher.publish(count.to_bytes(length=4, byteorder="big", signed=False))
            publisher.publish(str(count))

            count += 1

            if count > MAX_COUNT:
                count = 0

            print(count)

    except KeyboardInterrupt:
        controller.close_session()
        sys.exit(0)
    except Exception as e:
        print(e)
        sys.exit(-1)

def print_usage():
    print("Usage: python3 zenoh_test.py <pub/sub>")

def main():


    if len(sys.argv) != 2:
        print_usage()
        sys.exit(-1)

    controller = ZenohController()

    choice = sys.argv[1]

    if choice == "sub":
        create_test_subscriber(controller)
    elif choice == "pub":
        create_test_publisher(controller)
    else:
        print_usage()
        sys.exit(1)


if __name__ == "__main__":
    main()