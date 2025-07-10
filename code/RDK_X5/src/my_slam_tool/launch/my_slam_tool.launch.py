from launch import LaunchDescription
from launch_ros.actions import Node
 
def generate_launch_description():
    return LaunchDescription([
        Node(
            package='my_slam_tool',
            executable='async_slam_toolbox_node',
            output='screen',
            parameters=[{
                'use_sim_time': True  # 用仿真改为True，不用仿真就False
            }]
        )
    ])