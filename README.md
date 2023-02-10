# Explorer-Engine

Explorer Engine This is a simple Rendering Engine based on OpenGL and C++.

## 项目结构

### Explorer

- 引擎核心
- 编译后生成静态库文件和动态链接库（.dll）文件
- 在运行时链接到Sandbox
- 由Sandbox调用

### Sandbox

- 引擎应用启动项
- 最终编译为exe文件

## 开发过程

### 1. 项目入口点

- EntryPoint.h

### 2. 日志系统

- Log.h
- Log.cpp

### 3. Premake构建项目

- premake.lua

### 4. 实现事件系统

- Event.h
- ApplicationEvent.h
- KeyEvent.h
- MouseEvent.h

### 5. 创建预编译头文件

- exppch.h
- exppch.cpp

### 6. 窗口抽象和GLFW

- Window.h

### 7. 窗口事件

## 文件说明

### Explorer 项目

#### src

##### 预编译头文件

- exppch.h/.cpp。

  包含引擎需要用到的C++标准库文件和其他库文件，预先编译为二进制文件，加快项目构建速度。

##### 外部文件

- spdlog

  外部日志库文件。

- GLFW

  外部窗口库文件，配合OpenGL。

##### Events

* Event.h

  定义所有事件基类 Event、事件调度器 EventDispatcher，定义事件类型 EventType 和事件种类 EventCategory。

* ApplicationEvent.h

  定义所有窗口事件、App事件。

* KeyEvent.h

  定义所有键盘事件 KeyEvent。

* MouseEvent.h

  定义所有鼠标事件。

##### 内核文件

- Core.h

  引擎内核文件，定义引擎需要的宏和条件编译。

- Application.h/.cpp

  引擎应用程序入口，应用基类。定义所有事件函数，创建窗口并为窗口设置事件函数，进行事件调度。

- EntryPoint.h

  引擎入口点，负责创建应用程序。

-  Log.h/.cpp

  引擎日志系统，定义内核日志和客户端日志基本格式。

- Explorer.h

  引用Explorer.dll需要包含的所有头文件。

##### 窗口文件

- Window.h

  窗口类，可接收事件。

### Sandbox 项目

#### src

- SandboxApp.cpp

  应用程序类，继承Application

## 子系统

### 日志系统

### 事件系统

![](E:\毕业设计\Explorer\事件系统.png)

