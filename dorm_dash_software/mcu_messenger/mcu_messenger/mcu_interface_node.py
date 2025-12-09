import rclpy
from rclpy.node import Node, MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from std_msgs.msg import Float32MultiArray
from mcu_messenger.mcu_messaging import McuMessenger
from signal import signal, SIGINT, SIGTERM
from .parameter_handling import initialize_parameters
from numpy import clip
from rclpy.qos import qos_profile_sensor_data



class McuInterface(Node):

    NODE_NAME = "mcu_interface"
    MCU_READ_FREQ = 300 # HZ
    HEARTBEAT_FREQ = 1 # HZ

    SERIAL_INTERFACE_NAME = "/dev/AMA10"

    def __init__(self):

        super().__init__(self.NODE_NAME)

        signal(SIGINT, self.handle_exit_signal)
        signal(SIGTERM, self.handle_exit_signal)

        # TODO - should be parsed from a config file
        param_dict: dict = {
            # "temp_parameter": rclpy.Parameter.Type.BOOL,
        }

        self.config_params: "ConfigParams" = initialize_parameters(self, param_dict)

        if self.config_params is None:
            self.get_logger().fatal(f"Parameters did not initialize properly!")
            self.exit_safely()

        if not self.validate_parameters():
            self.get_logger().fatal(f"One or more parameters are invalid!")
            self.exit_safely()

        self.get_logger().info(f"Parameters Set: {self.config_params}")

        self.mcu_read_callback_group = MutuallyExclusiveCallbackGroup()
        self.mcu_heartbeat_callback_group = MutuallyExclusiveCallbackGroup()

        mcu_read_callback_dict: dict = {
            "string": func_example,
        }    

        self.mcu_messenger = McuMessenger(PLACEHOLDER, mcu_read_callback_dict)

        self.configure_mcu()

        # Define subscriptions ------------------------------------------------
        
        # Define publishers ---------------------------------------------------
        
        # Define services -----------------------------------------------------

        # Define timers -------------------------------------------------------

        self.heartbeat_timer = self.create_timer(
            1.0 / self.HEARTBEAT_FREQ,
            self.send_heartbeat,
            self.mcu_heartbeat_callback_group
        )

        self.mcu_recv_timer = self.create_timer(
            1.0 / self.MCU_READ_FREQ,
            self.handle_mcu_reading,
            self.mcu_read_callback_group
        )

    def handle_exit_signal(self, signum, frame):
        self.exit_safely()

    def exit_safely(self):
        raise SystemExit

    def validate_parameters(self) -> bool:
        # TODO - add to this
        return True

    def configure_mcu(self) -> None:
        self.mcu_messenger.configure_mcu(self.config_params)

    # Subscriber callbacks ----------------------------------------------------

    # Service callbacks -------------------------------------------------------
    
    def send_echo(self, request: EmptySrv.Request, response: EmptySrv.Response) -> EmptySrv.Response:
        time_stamp: int = self.get_clock().now().seconds_nanoseconds()[0]
        self.get_logger().info(f"echo sent: {time_stamp}")
        self.mcu_messenger.send_echo(time_stamp)
        return response

    # MCU read callbacks ---------------------------------------------------

    # Timer callbacks ---------------------------------------------------------

    def send_heartbeat(self) -> None:
        self.mcu_messenger.send_heartbeat_packet()

    def handle_mcu_reading(self) -> None:
        self.mcu_messenger.handle_mcu_payload()

def main(args=None):
    rclpy.init(args=args)

    mcu_messenger_node = McuInterface()
    executor = MultiThreadedExecutor()
    executor.add_node(mcu_messenger_node)

    try:
        executor.spin()
    except SystemExit:
        pass

    executor.remove_node(mcu_messenger_node)
    mcu_messenger_node.destroy_node()
    executor.shutdown()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
