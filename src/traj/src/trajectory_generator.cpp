#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "traj/msg/ref_pose.hpp"
#include "traj/msg/cartesian_state.hpp"

using namespace std::chrono_literals;

class MinimalNode : public rclcpp::Node{ 
    public :
        MinimalNode() : Node("trajectory_generator"), first_pose(false)
        {
            // Utilisation des majuscules pour le type C++
            publisher_ = this->create_publisher<traj::msg::CartesianState>("/desired_state", 10);
            subscriber_ = this->create_subscription<traj::msg::RefPose>("/nxt_pose", 10, std::bind(&MinimalNode::pose_callback, this, std::placeholders::_1));
            timer_ = this->create_wall_timer(50ms, std::bind(&MinimalNode::timer_callback, this));
        }

    private :
        rclcpp::Publisher<traj::msg::CartesianState>::SharedPtr publisher_;
        rclcpp::Subscription<traj::msg::RefPose>::SharedPtr subscriber_;
        rclcpp::TimerBase::SharedPtr timer_;
        bool first_pose;
        traj::msg::RefPose latest_pose_;

        void timer_callback(){
            if (!first_pose) {
                return;
            }
            auto state_msg = traj::msg::CartesianState();
            state_msg.x = latest_pose_.x;
            state_msg.y = latest_pose_.y;
            state_msg.vx = 2;
            state_msg.vy = 2;
            state_msg.ax = 2;
            state_msg.ay = 2;

            publisher_->publish(state_msg);
        }    

        void pose_callback(const traj::msg::RefPose::SharedPtr msg){
            if(!first_pose){
                RCLCPP_INFO(this->get_logger(), "PREMIERE POSE RECUE");
                first_pose = true;
            }
            latest_pose_ = *msg;
        }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalNode>());
    rclcpp::shutdown();
    return 0;
}