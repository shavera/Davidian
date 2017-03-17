# Davidian
Calculator to analyze different orbits for slingshot behaviour

In Kerbal Space Program, I'd like the information to execute slingshot maneuvers that will exit in or near desired orbits. As a start to that process, a program will need to be able to simulate a system of celestial bodies (fixed orbits with 'spheres of influence' patched conic gravitation) and the motion of one free body as it passes through the system. 

Each data calculation run will consist of placing the free body at some position (Mean Anomaly) in its orbit for t=0, and giving it a delta-v 'kick' of varying strength and direction. The program will then follow the free body as it passes through its orbit and into the orbit of any other bodies to see what kind of final orbit the free body maintains.

Design plan:
Phase 1: Program can calculate a database of state vectors (position and velocity) for all celestial bodies in the program.
Phase 2: Program can calculate a 2-body (celestial) 2-dimensional system with one free body moving between the two bodies.
Phase 3: Program can calculate a 2-body 3-d system with a free body
Phase 4: Program can calculate many-body 3-d system with free body
Phase 5: Program can read in experimental parameter settings and only keep orbits that fit some desired flight path
Future: many-body system will be parameterized so that each body can be shifted in its orbit for timing

Program execution design:
1. program will pre-calculate or 'lazy-calculate' a database of celestial body state vectors
  * pre-calculation may be faster, but lazy calculation allows us to only calculate what we need as we need it instead of guessing at overall flight times for a pre-calculation.
2. program will 'kick' free body out of its orbit and time step through its new orbit
3. at each time step, program will test to see if free body is now within some other body's sphere of influence or has exited its current body's sphere of influence.
4. program will check to see if flight plan is successful or if some alternate end criteria are met.
  * end criteria and flight plans will be hard coded until phase 5.
5. program will record final orbital parameters to a database keyed by the initial conditions (state vector of free body, vector of delta-v kick).

The data produced by this will need to be represented in some graphical form, which the davidian-qt repository will likely handle.
