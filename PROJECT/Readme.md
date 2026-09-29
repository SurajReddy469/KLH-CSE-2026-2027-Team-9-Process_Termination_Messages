# Process Termination Messages

## 📌 About the Project

**Process Termination Messages** is a browser-based Operating Systems project designed to demonstrate important process-management concepts through an interactive web interface.

The project simulates the complete lifecycle of a process, starting from process creation and execution, followed by monitoring and different types of process termination. It also demonstrates signal handling, exit codes, termination status, and meaningful termination messages.

The application is developed using **HTML5, CSS3, and JavaScript** and can be executed directly in a web browser.

> **Important:** This project is a frontend simulation. It represents Operating Systems process-management concepts through JavaScript and does not directly create or terminate real operating-system processes.

---

# 🎯 Objectives

The main objectives of the project are:

1. To demonstrate the concept of process creation.
2. To simulate process execution and monitoring.
3. To demonstrate different process termination conditions.
4. To display process status and termination information.
5. To demonstrate signal-based termination.
6. To show exit codes associated with different termination scenarios.
7. To provide meaningful termination messages.
8. To provide an interactive interface for testing process-management concepts.
9. To make Operating Systems process concepts easier to understand through visualization.

---

# ⭐ Main Features

The project provides the following features:

* Process Creation
* Process Execution
* Process Monitoring
* Process Termination
* Normal Termination
* Error Termination
* User Termination
* Signal Termination
* Signal Handling
* Exit Code Display
* Termination Status
* Termination Messages
* Event Logging
* Testing & Error Identification
* Interactive Dashboard
* Responsive User Interface

---

# 🧩 Project Modules

## 1. Process Creation

This module allows the user to create a simulated child process.

The user can provide information such as:

* Process Name
* Parent PID
* Task

The application generates a simulated Process ID (PID) and adds the process to the process list.

---

## 2. Process Execution

This module allows a created process to be executed.

During execution, the interface displays:

* Process name
* Process status
* Execution progress
* Runtime

The simulated process changes through different states.

```text
Created
   ↓
Running
   ↓
Completed
```

---

## 3. Process Monitoring

The monitoring module provides information about the currently created processes.

The displayed information includes:

* PID
* PPID
* Process Name
* Status
* Runtime
* Reason

This allows the user to observe the current state of each simulated process.

---

## 4. Process Termination

The project supports different termination scenarios.

### Normal Termination

Represents successful process completion.

```text
Status: Completed
Exit Code: 0
```

### Error Termination

Represents termination caused by an execution error.

```text
Status: Terminated
Exit Code: 1
```

### User Termination

Represents termination caused by user interruption.

```text
Signal: SIGINT
Exit Code: 130
```

### Signal Termination

Represents termination caused by a termination signal.

```text
Signal: SIGTERM
Exit Code: 143
```

---

# 📡 Signal Handling

The project provides a simulated signal-handling interface.

The available signals include:

| Signal  | Purpose                        |
| ------- | ------------------------------ |
| SIGTERM | Controlled process termination |
| SIGINT  | User interruption              |
| SIGKILL | Immediate process termination  |

When a signal is selected, the process status is updated and the corresponding signal and exit code are displayed.

---

# 📊 Termination Status

The **Termination Status** section displays information about the latest process termination.

Information can include:

```text
PID
PPID
Status
Exit Code
Signal
Reason
```

Example:

```text
PID: 1001
PPID: 1000
Status: Terminated
Exit Code: 143
Signal: SIGTERM
Reason: Process terminated by signal SIGTERM.
```

---

# 💬 Termination Messages

The project generates meaningful messages according to the process termination condition.

Examples:

```text
Process completed successfully.
```

```text
Process terminated due to an execution error.
```

```text
Process terminated by user action (SIGINT).
```

```text
Process terminated by signal SIGTERM.
```

These messages help users understand why a process stopped.

---

# 🧪 Testing & Error Identification

The project includes a testing section that provides predefined scenarios.

### Test 1 — Normal Completion

Expected:

```text
Status: Completed
Exit Code: 0
```

### Test 2 — Execution Error

Expected:

```text
Status: Terminated
Exit Code: 1
```

### Test 3 — User Termination

Expected:

```text
Status: Terminated
Signal: SIGINT
Exit Code: 130
```

### Test 4 — Signal Termination

Expected:

```text
Status: Terminated
Signal: SIGTERM
Exit Code: 143
```

The test results are displayed through the application's termination status and termination message sections.

---

# 🔄 Project Workflow

The complete project workflow is:

```text
                    User
                     │
                     ▼
              Process Creation
                     │
                     ▼
              Process Execution
                     │
                     ▼
             Process Monitoring
                     │
                     ▼
             Process Termination
                     │
          ┌──────────┼──────────┐
          │          │          │
          ▼          ▼          ▼
       Normal      Error      Signal
          │          │          │
          └──────────┼──────────┘
                     │
                     ▼
             Termination Status
                     │
                     ▼
            Termination Message
                     │
                     ▼
                  Result
```

---

# 📁 Project Structure

```text
Process-Termination-Messages/
│
├── src/
│   ├── index.html
│   │
│   ├── css/
│   │   └── style.css
│   │
│   └── js/
│       └── script.js
│
├── data/
│   ├── README.md
│   ├── process_test_data.csv
│   ├── process_results.txt
│   └── sample_processes.csv
│
├── Result/
│   └── README.md
│
└── README.md
```

---

# 🛠️ Technologies Used

### Frontend

* HTML5
* CSS3
* JavaScript

### Development Environment

* Visual Studio Code
* Web Browser
* Live Server

---

# 📄 Source Code

The `src` folder contains the main application source code.

### `index.html`

Provides the structure of the application and contains the different project sections.

### `css/style.css`

Controls the appearance, layout, status indicators, cards, tables, buttons, and responsive design.

### `js/script.js`

Contains the application logic for:

* Process creation
* Process execution
* Process monitoring
* Process termination
* Signal simulation
* Exit codes
* Testing
* Termination messages
* Dynamic interface updates

---

# 📂 Data Folder

The `data` folder contains supporting sample information used for project demonstration and testing.

It includes:

* Process test data
* Sample process information
* Sample termination results

The data represents the simulated process states and termination scenarios used by the project.

---

# 📸 Result Folder

The `Result` folder is used for storing project demonstration evidence.

It can contain:

* Screenshots
* Test results
* Process creation results
* Process execution results
* Process monitoring results
* Termination results
* Signal-handling results

---

# ▶️ How to Run the Project

## Method 1 — Browser

1. Open the project folder.
2. Open the `src` folder.
3. Open:

```text
index.html
```

4. The application will open in your default browser.

---

## Method 2 — VS Code Live Server

1. Open the project in **Visual Studio Code**.
2. Open the `src` folder.
3. Open `index.html`.
4. Right-click on `index.html`.
5. Select **Open with Live Server**.
6. The application will open in your browser.

No additional installation or backend server is required.

---

# 🖥️ How to Demonstrate the Project

### Step 1 — Create a Process

Open **Process Creation**.

Enter the process information and create the process.

### Step 2 — Execute the Process

Go to **Process Execution**.

Select the process and start execution.

### Step 3 — Monitor

Open **Process Monitoring** and show:

* PID
* PPID
* Status
* Runtime
* Reason

### Step 4 — Terminate

Demonstrate different termination options:

* Normal
* Error
* User
* Signal

### Step 5 — Check Result

Open **Termination Status** and **Termination Messages**.

Explain:

* Process status
* Exit code
* Signal
* Termination reason

### Step 6 — Run Tests

Open **Testing & Error Identification** and demonstrate the predefined test cases.

---

# 📋 Expected Results

| Scenario           | Status     | Signal  | Exit Code |
| ------------------ | ---------- | ------- | --------: |
| Normal Termination | Completed  | —       |         0 |
| Error Termination  | Terminated | —       |         1 |
| User Termination   | Terminated | SIGINT  |       130 |
| Signal Termination | Terminated | SIGTERM |       143 |
| SIGKILL            | Terminated | SIGKILL |       137 |

---

# ⚠️ Limitations

This project is implemented as a **browser-based simulation**.

It does not directly execute real Linux process-management system calls such as:

```text
fork()
execvp()
waitpid()
kill()
exit()
```

Instead, JavaScript simulates:

* Process IDs
* Process states
* Process execution
* Signals
* Exit codes
* Termination conditions
* Termination messages

The project is therefore intended for **visual demonstration and learning of Operating Systems process-management concepts**.

---

# 🎓 Operating Systems Concepts Demonstrated

The project demonstrates the following concepts:

* Process Creation
* Parent and Child Processes
* Process Execution
* Process States
* Process Monitoring
* Process Termination
* Exit Status
* Signals
* Signal Handling
* Error Handling
* Process Synchronization Concepts
* Process Lifecycle

---

# 👥 Team Members

| Roll Number | Name               |
| ----------- | ------------------ |
| 2520030355  | **Ch Suraj Reddy** |
| 2520030387  | **Vanka Vijay**    |
| 2520030213  | **Kevin Josh**     |

---

# ✅ Conclusion

The **Process Termination Messages** project provides an interactive way to understand process-management concepts from Operating Systems.

Through the different modules, users can create simulated processes, execute and monitor them, terminate them using different methods, handle simulated signals, and view detailed termination information.

The project combines **HTML, CSS, and JavaScript** to provide a simple and interactive environment for demonstrating process lifecycle and termination concepts.
