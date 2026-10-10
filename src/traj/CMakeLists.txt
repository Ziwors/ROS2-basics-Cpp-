cmake_minimum_required(VERSION 3.8)
project(traj)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 1. Déclaration de tes fichiers de messages personnalisés
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RefPose.msg"
  "msg/CartesianState.msg")

# 2. Création de l'exécutable pour ton nœud C++
add_executable(trajectory_generator src/trajectory_generator.cpp)
ament_target_dependencies(trajectory_generator rclcpp)

# --- SECURITÉ IMPORTANTE ---
# Force CMake à générer les messages AVANT de compiler le nœud C++
add_dependencies(trajectory_generator ${PROJECT_NAME})

# 3. Ligne magique indispensable pour lier les messages au nœud C++
rosidl_target_interfaces(trajectory_generator
  traj "rosidl_typesupport_cpp"
)

# 4. Installation des exécutables
install(TARGETS
  trajectory_generator
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()