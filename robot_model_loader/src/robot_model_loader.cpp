/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2011, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#include "robot_model_loader/robot_model_loader.h"

robot_model_loader::RobotModelLoader::RobotModelLoader(const std::string &robot_description) : nh_("~")
{
    if (nh_.searchParam(robot_description, robot_description_))
        loadRobotFromParamServer();
}

bool robot_model_loader::RobotModelLoader::loadRobotFromParamServer(void)
{
    std::string content;
    if (nh_.getParam(robot_description_, content))
    {
        urdf_.reset(new urdf::Model);
        if (urdf_->initString(content))
        {
            std::string scontent;
            if (nh_.getParam(robot_description_ + "_semantic", scontent))
            {
                srdf_.reset(new srdf::Model);
                if (!srdf_->initString(*urdf_, scontent))
                {
                    ROS_ERROR("Unable to parse SRDF");
                    srdf_.reset();
                }
            }
            else
                ROS_ERROR("Robot semantic description not found. Did you forget to define or remap '%s_semantic'?", robot_description_.c_str());
        }
        else
        {
            ROS_ERROR("Unable to parse URDF");
            urdf_.reset();
        }
    }
    else
        ROS_ERROR("Robot model not found! Did you remap '%s'?", robot_description_.c_str());
    return false;
}