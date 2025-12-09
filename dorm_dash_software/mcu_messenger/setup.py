from setuptools import setup

PACKAGE_NAME = 'mcu_messenger'

PROTOBUF_MESSAGES_FILE = 'messages/dormdash_pb2.py'

setup(
    name=PACKAGE_NAME,
    version='0.0.0',
    packages=[PACKAGE_NAME],
    data_files=[ # Include in share dir
        ('share/ament_index/resource_index/packages',
            ['resource/' + PACKAGE_NAME]),
        ('share/' + PACKAGE_NAME, ['package.xml']),
    ],
    package_data  = { # Include in build
        'mcu_messenger': [PROTOBUF_MESSAGES_FILE],
    },
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Cameron Rosenthal',
    maintainer_email='cameronaustinrosenthal@gmail.com',
    description='Dorm Dash MCU communication package.',
    license='TODO',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            f'mcu_interface = {PACKAGE_NAME}.mcu_interface_node:main',
        ],
    },
    
)

# Good practice to run rosdep at root of workspace to
# check for missing dependencies before building.
# rosdep install -i --from-path src --rosdistro humble -y
