import zenoh
from zenoh import Config, Session, Reliability as ZenohReliability, \
Encoding, CongestionControl, Priority, Locality, Publisher, Subscriber, Sample
from enum import Enum

class Reliability(Enum):
    RELIABLE = ZenohReliability.RELIABLE
    BEST_EFFORT = ZenohReliability.BEST_EFFORT

class ZenohPublisher():

    def __init__(self, session: Session, publisher: Publisher):
        self.session_context = session
        self.publisher = publisher

    def publish(self, data) -> bool:
        if not self.session_context.is_closed():
            self.publisher.put(data)

class ZenohController():

    def __init__(self, config_file: str = None):
        
        self.zenoh_config = Config.from_file(config_file) if config_file else Config()
        self.session = None

        self.publisher_list: list[Publisher] = []
        self.subscriber_list: list[Subscriber] = []

    def __del__(self):
        self.close_session()

    def open_session(self) -> bool:

        self.publisher_list.clear()
        self.subscriber_list.clear()

        self.session: Session = zenoh.open(self.zenoh_config)
        return self.session is not None

    def close_session(self) -> None:
        if self.session is not None:
            self.session.close()
    
    # TODO - generalize
    def create_publisher(self, key: str, reliability: Reliability):

        publisher: Publisher = self.session.declare_publisher(
            key_expr=key,
            encoding=Encoding.APPLICATION_PROTOBUF,
            congestion_control=CongestionControl.DEFAULT,
            allowed_destination=Locality.DEFAULT,
            reliability=reliability.value
        )

        return ZenohPublisher(self.session, publisher)

    def create_subscriber(self, key: str, callback):
        
        def callback_wrapper(sample: Sample):
            callback(sample.payload)

        subscriber: Subscriber = self.session.declare_subscriber(
            key_expr=key, 
            handler=callback_wrapper,
            allowed_origin=Locality.DEFAULT
        )

        self.publisher_list.append(subscriber)