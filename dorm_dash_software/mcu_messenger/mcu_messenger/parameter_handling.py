import rclpy
from rclpy.node import Node
from rclpy.exceptions import InvalidParameterTypeException, ParameterUninitializedException
from rclpy.parameter import parameter_value_to_python
from collections import namedtuple

def initialize_parameters(node: Node, param_dict: dict[str, rclpy.Parameter.Type]) -> None:

        """
        Declare and initialize ROS2 parameters.
        param_dict is a dict containing {"parameter_name": rclpy.Parameter.Type}
        Returns a namedtuple containing class members of parameter names.
        Ensures parameters are all initialized, ensures correct type.

        Example:

        param_dict: dict = {
            "max_limit": rclpy.Parameter.Type.DOUBLE,
            "min_limit": rclpy.Parameter.Type.DOUBLE,
        }

        """

        # Declare parameters
        for param_name in param_dict.keys():
            try:
                node.declare_parameter(param_name, param_dict.get(param_name))
            except InvalidParameterTypeException:
                node.get_logger().fatal(f"Invalid parameter type: '{param_name}'")
                return None

        ConfigParams = namedtuple("ConfigParams", param_dict.keys())

        # Ensure all parameters are set
        for param_name in param_dict.keys():
            try:
                param_val = parameter_value_to_python(node.get_parameter(param_name).get_parameter_value())
                param_dict[param_name] = param_val # Replace type with val
            except ParameterUninitializedException:
                node.get_logger().fatal(f"Parameter not set: '{param_name}'")
                return None

        return ConfigParams(**param_dict)
