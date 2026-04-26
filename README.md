#  Snake Game

This is a simple **Snake Game** built using C++ and Qt, designed to demonstrate basic game development concepts along with Data Structures and Algorithms (DSA), mainly the use of a **Linked List**.

---

##  What the Game Does

The player controls a snake that moves around a grid. The main goal is to eat food and grow longer while avoiding collisions with walls or the snake itself.

Each time the snake eats food, its length increases, making the game more challenging.

---

##  DSA Concept Used

The snake is implemented using a **Singly Linked List**:

- Each segment of the snake is a **node**
- The head represents the front of the snake
- Each node points to the next segment
- When the snake moves:
  - A new node is added at the head
  - The last node (tail) is removed (unless food is eaten)

This allows smooth and dynamic movement of the snake.

---

##  Other Concepts Used

- Grid-based game board (2D structure)
- Collision detection (walls and self-collision)
- Random food generation
- Object-Oriented Programming (OOP)
- Game loop / timer for continuous movement
- Qt framework for GUI and rendering

---

##  Features

- Smooth snake movement
- Score increases as snake eats food
- Game over on collision
- Simple and clean UI
- Increasing difficulty as snake grows

---

##  Purpose of the Project

This project was created to understand:
- How linked lists work in real-world applications
- Basic game development logic
- Event-driven programming using Qt
- Implementation of DSA concepts in interactive applications

---
