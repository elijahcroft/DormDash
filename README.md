# Dorm Dash – Dorm Delivery Robot

## Project Overview
Dorm Dash is a dorm hall delivery robot designed for CSUF. The goal is to create an autonomous robot that can:  
- Safely traverse a dorm hallway  
- Avoid collisions with obstacles  
- Deliver payloads to specific dorm rooms  

This project explores robotics, embedded systems, and web-based interfaces for autonomous delivery in structured indoor environments.

---

## Project Goals
- Autonomous navigation inside dorm halls  
- Accurate delivery to designated dorm doors  
- User-friendly interface for sending and tracking deliveries  

---

## Wish / Prefer / Demand
- **Wish**: A door knocker or bell that activates when delivery is completed  
- **Prefer**: A web-based interface for interacting with the robot  
- **Demand**: Core delivery and navigation functionality  

---

## System Design

### Sensor Suite
- Single-point LiDAR (for wall/obstacle avoidance)  
- 6-DoF IMU for localization and orientation  
- Aruco tags for precise docking and absolute position reference  

### Compute
- Raspberry Pi 5 (4GB RAM) running Ubuntu 22  

### Motors & Motor Control
- TBA  

### Battery
- TBA  

### Chassis Hardware
- TBA  

---

## Web Interface & Backend

### Features
**Submission Request Queue**  
- Users enter:  
  - Item description  
  - Source dorm number  
  - Destination dorm number  
- Confirm → Robot travels to source location  
- Robot waits for item to be placed → User confirms → Robot departs  

**Shipment Progress**  
- Real-time tracking with either:  
  - Robot location on map  
  - Distance to destination  

---

## Robot Navigation
- **Mapping & Waypoints**  
  - Pre-mapped dorm hallways  
  - Positional waypoints added for each dorm room  

- **Localization**  
  - IMU-based odometry  
  - Aruco tags for absolute position correction  

- **Obstacle Avoidance**  
  - Single-point LiDAR for wall-following and obstacle detection  

- **Docking & Delivery**  
  - Aruco tags at dorm doors for final approach accuracy  
  - Aruco tag at start location for global position reset  

---

## Future Improvements
- Integration of a door-knocking/notification system  
- Optimization of path planning for multi-delivery runs  
- Expansion to multi-floor navigation with elevators  

---

## Course Info
- **Class**: CSPC 362 – Project Design  
- **Project**: Dorm Delivery Robot (Dorm Dash)  
- **University**: California State University, Fullerton (CSUF)  
