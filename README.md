# EasyRTOS
Real-time operating system for microcontrollers

#### Description: 
EasyRTOS is a simple real-time operating system for microcontrollers. EasyRTOS is designed for IoT projects. But it can be used for other purposes as well.

#### Available functionality:
* Scheduler and task switching mechanism.
* Mutexes (signal, wait, lock, unlock) - to control the flow of execution and build logic for switching tasks.
* Queues.
* Lists.

#### Planned functionality:
* Timers.
* Interrupt handling.
* Library for transferring data to the cloud.


A workspace is provided in the repository. Inside there is a project that demonstrates the work of EasyRTOS. To run the project, you need to install the [Atollic TrueSTUDIO for STM32](https://atollic.com/truestudio/) development environment version 9.3.0 or higher. Select the folder with the repository as workspace. The EasyRTOS project should appear in the list of projects. Next, you need to configure target for the project. In this case, the test project was developed on the STM32F4DISCOVERY debug board.
