# Interrupt‑Driven Event Logger (C++)

This project demonstrates a simple **event‑driven system** written in C++.  
One thread simulates an interrupt by generating events, and the main program
processes those events using a thread‑safe queue.

The focus is on showing how interrupts can signal events quickly while actual
work is handled later in a main loop.

## What This Project Shows
- Interrupt‑style event generation using threads  
- Safe communication between threads with a queue  
- Mutexes and condition variables for synchronization  
- Clean thread startup and shutdown  
- Basic event latency measurement
