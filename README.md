# The Boys - Discrete Event Simulation

This project is a discrete event simulator (SED) developed for the **CI1001 Programming 1** course at **UFPR**.

* **Student:** Lucas Mazurkievicz Sekikawa
* **Professor:** Marcos Castilho

## Overview

The simulation models a world inhabited by heroes, bases, and missions, where system dynamics evolve through an event agenda over a simulated timeline of one year (525,600 minutes). The world is represented on a Cartesian plane where each unit corresponds to one meter.

### Core Entities

* **Heroes:** Defined by a unique ID, a set of skills, movement speed, and a patience level.
* **Bases:** Fixed locations with a maximum capacity and a waiting queue managed by a doorman.
* **Missions:** Events requiring a specific set of skills to be completed.
* **Compound V:** A limited resource that allows an experienced hero to complete a mission instantly if no team is available, resulting in the hero's death after use.

## Simulation Logic

The engine operates using a **Future Event List (LEF)**, which is a priority queue ordered by time. The simulator processes events in sequence, updating the global clock and the state of the system.

### Implemented Events

* **ARRIVE / WAIT / LEAVE:** Handling hero movement and base queueing decisions based on patience.
* **ADMIT / ENTER:** Management of base occupancy and stay duration.
* **TRAVEL:** Distance calculations and travel time based on hero speed.
* **MISSION:** Skill check and team selection for completing objectives.
* **DIE:** Removal of heroes from the world.

## Project Structure

The project is modularized into several components and Abstract Data Types (ADTs):

* `theboys.c`: Main program and simulation loop.
* `eventos.h/c`: Implementation of individual event logic.
* `entidades.h`: Definition of Hero, Base, Mission, and World structures.
* `inicia.h/c`: World and entity initialization.
* `destroi.h/c`: Memory cleanup and deallocation.
* `lef.h/c`: Management of the Future Event List.
* `fprio.h/c`: Priority Queue ADT.
* `conjunto.h/o`: Set ADT for managing skills and collections.
* `lista.h/c` & `fila.h/c`: List and Queue ADTs for base waiting lines.
* `makefile`: Compilation script.

## Compilation and Execution

The project must be compiled using the provided `makefile`, which includes the mandatory flags `-Wall -Werror -Wextra -std=c99 -g`.

To compile the project:
```bash
make

To run the simulation:
./theboys

## Results and Statistics
* `Hero Statistics`: Final status (alive or dead) , experience points , and final skill sets.
* `Base Statistics`: Maximum occupancy (capacity) , maximum queue size reached , and total missions completed.
* `Global Data`: Total number of events processed , mission success rate , and overall hero mortality rate.
