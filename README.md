# AiSwGenRepo

This repository contains multiple sub-projects with unit implementations, documentation, and build configurations.  
Unit tests are managed using **Ceedling**, and documentation is generated with **Doxygen**.

---

## 📂 Directory Structure

---

### 🔹 Projects

#### 1. Calc_ecuTemperature_s16
- `docs/html/index.html` → Graphical visualization of Doxygen description  
- `src/`  
  - `file.c` → Implementation of the unit  
  - `file.h` → Doxygen description of the unit  
- `Makefile` → Builds the project (`file.c`, `file.h`)  
- `Doxyfile` → Generates documentation inside `docs/html`

---

#### 2. CurrentMon_Derating
- `docs/html/index.html` → Graphical visualization of Doxygen description  
- `src/`  
  - `file.c` → Implementation of the unit  
  - `file.h` → Doxygen description of the unit  
- `Makefile` → Builds the project (`file.c`, `file.h`)  
- `Doxyfile` → Generates documentation inside `docs/html`

---

#### 3. NvmMngr_WriteRequest_
- `docs/html/index.html` → Graphical visualization of Doxygen description  
- `src/`  
  - `file.c` → Implementation of the unit  
  - `file.h` → Doxygen description of the unit  
- `Makefile` → Builds the project (`file.c`, `file.h`)  
- `Doxyfile` → Generates documentation inside `docs/html`

---

#### 4. TempMon_Run
- `docs/html/index.html` → Graphical visualization of Doxygen description  
- `src/`  
  - `file.c` → Implementation of the unit  
  - `file.h` → Doxygen description of the unit  
- `Makefile` → Builds the project (`file.c`, `file.h`)  
- `Doxyfile` → Generates documentation inside `docs/html`

---

### 🔹 Root-Level Files
- `Makefile` → Launches the build process for all sub-projects

---

## ⚙️ Build Instructions

To build all sub-projects inside a Docker container, run the following command:

```bash
docker run -it --rm -v C:\_YOUR_PATH_\project:/usr/src/app -w /usr/src/app gcc:latest make
