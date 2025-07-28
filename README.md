# IPL_Scheduling

## 📌 Project Overview

The IPL Scheduling project is designed to generate a **fair and balanced match schedule** for the Indian Premier League (IPL), ensuring:

- Equal distribution of home/away games
- Minimum player fatigue
- Optimal venue usage and broadcasting time slots
- Prioritization of prime-time matches for high-profile teams

---

## 🎯 Objectives

- Develop an efficient IPL scheduling system
- Avoid consecutive matches for the same team
- Allocate venues based on team/ground availability
- Assign matches to time slots for maximum viewership
- Maintain fairness across all teams
- Handle external constraints (holidays, broadcasting rules, etc.)

---

## 🛠️ Techniques Used

- Modified Double Round Robin Algorithm
- Dijkstra's Algorithm
- Bubble Sort Algorithm
- Array of Structures
- Pointers and File Handling
- Zenie’s Algorithm (for optimal slot assignment)

---

## 🧩 Modules

- **Input Module:** Read teams, venues, match details
- **Scheduling Module:** Generate fixtures using custom algorithms
- **Management Module:** Update points and match outcomes
- **Results Module:** Display results and standings
- **Playoffs Module:** Determine playoff teams
- **Data Input/Export Module:** Handle file I/O
- **Search Module:** Search teams/venues/matches
- **Shortest Distance Module:** Venue optimization using Dijkstra

---

## 🧱 Architecture

The system includes:
- Data Input → Match Schedule Generation → Match Results → Points Table → Output Display
- Modules interact using structured data and file-based input/output
- Flow includes User Interface Simulation and Text-based Data Logs

---

## 🧠 Learning Outcomes

- Applied algorithm design and data structure concepts
- Gained practical experience in C with arrays, linked lists, and structures
- Developed teamwork, debugging, and version control skills
- Strengthened critical thinking and problem solving
- Learned event and schedule management dynamics

---

## ▶️ How to Run

### 1. **Compile the Code**
Make sure you have GCC installed. Run the following command in your terminal:

```bash
gcc input.c matches.c playoffs.c Points_table.c schedule.c search_module.c ShortestDistance.c teams.c -o scheduling
