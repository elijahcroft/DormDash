from mcu_messenger.messages.dormdash_pb2 import HostToMcu, McuToHost
import logging
from google.protobuf.message import DecodeError
from enum import Enum
from rclpy.logging import get_logger

LOGGER_NAME = "mcu_messaging"   

class McuMessenger:

    def __init__(self, controller_placeholder, mcu_read_callbacks: dict):
        self.controller_placeholder = controller_placeholder
        self.mcu_read_callbacks = mcu_read_callbacks

    def send_echo(self, time_stamp: int) -> None:
        payload = HostToMcu()
        payload.send_echo.time_stamp = time_stamp
        self.send_host2mcu_payload(payload, self.controller_placeholder.zenoh_pub.publish)

    def send_heartbeat_packet(self) -> None:
        payload = HostToMcu()
        payload.send_heartbeat_packet.SetInParent()
        self.send_host2mcu_payload(payload, self.controller_placeholder.zenoh_pub.publish)

    def configure_mcu(self, config_params: "ConfigParams") -> None:
        ...
        # payload = HostToMcu()

        # payload.set_mcu_config.example_param = config_params.example_param

        # self.send_host2mcu_payload(payload, self.socket_controller.zenoh_pub.publish)

    @staticmethod
    def send_host2mcu_payload(payload: HostToMcu, send_func) -> None:
        send_func(payload.SerializeToString())
    
    @staticmethod
    def receive_mcu2host_payload(recv_func, buf_len: int) -> McuToHost:

        buf = recv_func(buf_len)

        payload = McuToHost()

        if len(buf) == 0:
            return None
        
        try:
            payload.ParseFromString(buf)
        except DecodeError:
            logging.getLogger(LOGGER_NAME).warning("mcu2host decode error!")
            return None
        
        return payload
    
    def handle_mcu_payload(self) -> None:
        
        payload = McuMessenger.receive_mcu2host_payload(self.controller_placeholder.zenoh_pub.recv_packet, 1024)
        
        if payload is None:
            return
        
        # Find which field in the OneOf is set
        field_name = payload.WhichOneof("payload")
        
        match field_name:

            case "echo_reply":
                self.mcu_read_callbacks["echo_reply"](payload.echo_reply.time_stamp)
            case _:
                get_logger(LOGGER_NAME).error(f"MCU read payload: \"{field_name}\" is not implemented!")